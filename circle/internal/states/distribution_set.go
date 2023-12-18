package states

type DistributionSet struct {
	Resolution  int
	Sensitivity int
	D           Matrix[Distribution]
}

func MakeDistributionSet(resolution int, sensitivity int) DistributionSet {
	ds := NewMatrix[Distribution](resolution, sensitivity)
	ds.ForEach(func(i int, j int) {
		ds[i][j] = MakeDistribution(i+1, j+1)
	})

	d := DistributionSet{resolution, sensitivity, ds}

	return d
}

func (d *DistributionSet) Val(s State) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)
	r.ForEach(func(i int, j int) {
		r[i][j] = d.D[i][j].Val(s)
	})
	return r
}

func (d *DistributionSet) Inc(s State, n int) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)
	r.ForEach(func(i int, j int) {
		r[i][j] = d.D[i][j].Inc(s, n)
	})
	return r
}
