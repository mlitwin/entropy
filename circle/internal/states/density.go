package states

type State []int

func (d *Density) stateHash(s State) State {
	sub := make(State, len(s))
	for i, v := range s {
		sub[i] = v / d.Sensitivity
	}
	return sub
}

type Density struct {
	Sensitivity int
	m           SliceMap[int]
	n           int
}

func MakeDensity(sensitivity int) Density {
	d := Density{}
	d.Sensitivity = sensitivity
	d.m = MakeSliceMap[int](new(IntIncrementer))

	return d
}
func (d *Density) Val(s State) int {
	h := d.stateHash(s)
	return d.m.Val(h)
}

func (d *Density) Inc(s State, n int) int {

	h := d.stateHash(s)
	d.n += n

	return d.m.Inc(h, n)

}
