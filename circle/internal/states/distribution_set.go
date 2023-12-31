package states

type sensitivitySet struct {
	ptrs []int
}

type sensitivitySetIncrementer struct {
	s []SliceMap[int]
}

func (si sensitivitySetIncrementer) Inc(x *sensitivitySet, n int) {

	for i := range si.s {
		si.s[i].IncPtr(x.ptrs[i], n)
	}

}

func (si sensitivitySetIncrementer) Construct(ss *sensitivitySet, m Measurement) {
	n := len(si.s)
	ss.ptrs = make([]int, n)
	tmp := make([]int, len(m))
	for i := range ss.ptrs {
		for k := range tmp {
			tmp[k] = m[k] / (i + 1)
		}
		ss.ptrs[i] = si.s[i].ValPtr(tmp)
	}
}

func MakeSensitivitySetIncrementer(sensitivity int) sensitivitySetIncrementer {
	var si sensitivitySetIncrementer

	si.s = make([]SliceMap[int], sensitivity)
	for i := range si.s {
		si.s[i] = MakeSliceMapOfInt()
	}

	return si
}

type DistributionSet struct {
	Resolution  int
	Sensitivity int
	D           []SliceMap[sensitivitySet]
}

func MakeDistributionSet(resolution int, sensitivity int) DistributionSet {

	d := DistributionSet{resolution, sensitivity, nil}

	d.D = make([]SliceMap[sensitivitySet], resolution)
	for i := range d.D {
		d.D[i] = MakeSliceMap[sensitivitySet](MakeSensitivitySetIncrementer(sensitivity))
	}
	return d
}

func (d *DistributionSet) Val(s State) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)
	for i := 0; i < d.Resolution; i++ {
		v := d.D[i].Val(s)

		for j := 0; j < d.Sensitivity; j++ {
			fr := d.D[i].incrementer.(sensitivitySetIncrementer)
			r[i][j] = fr.s[j].ValFromPtr(v.ptrs[j])
		}
	}
	return r
}

func (d *DistributionSet) Inc(s State, n int) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)

	for i := 0; i < d.Resolution; i++ {
		v := d.D[i].Inc(s, n)

		for j := 0; j < d.Sensitivity; j++ {

			r[i][j] = v.ptrs[j]
		}
	}

	return r
}
