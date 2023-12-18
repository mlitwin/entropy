package states

type Distribution struct {
	Resolution  int
	Sensitivity int
	D           Density
}

func makeSubstate(s State, resolution int, sensitivity int) (sub State) {
	n := len(s) / resolution
	if n*resolution < len(s) {
		n++
	}

	sub = make([]int, n)
	for i := range sub {
		t := 0
		x := i * resolution
		for x < (i+1)*resolution && x < len(s) {
			t += s[x]
			x++
		}
		sub[i] = t / sensitivity
	}
	return
}

func MakeDistribution(resolution int, sensitivity int) Distribution {
	d := Distribution{resolution, sensitivity, MakeDensity()}

	return d
}

func (d *Distribution) Val(s State) int {
	sub := makeSubstate(s, d.Resolution, d.Sensitivity)
	return d.D.Val(sub)
}

func (d *Distribution) Inc(s State, n int) int {
	sub := makeSubstate(s, d.Resolution, d.Sensitivity)
	return d.D.Inc(sub, n)
}

type DistributionSet struct {
	Resolution  int
	Sensitivity int
	D           Matrix[Distribution]
}

func MakeDistributionSet(resolution int, sensitivity int) DistributionSet {
	ds := NewMatrix[Distribution](resolution, sensitivity)
	for x := 0; x < ds.N(); x++ {
		for v := 0; v < ds.M(); v++ {
			ds[v][x] = MakeDistribution(x+1, v+1)
		}
	}
	d := DistributionSet{resolution, sensitivity, ds}

	return d
}

func (d *DistributionSet) Val(s State) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)
	for x := 0; x < r.N(); x++ {
		for v := 0; v < r.M(); v++ {
			r[v][x] = d.D[v][x].Val(s)
		}
	}
	return r
}

func (d *DistributionSet) Inc(s State, n int) Matrix[int] {
	r := NewMatrix[int](d.Resolution, d.Sensitivity)
	for x := 0; x < r.N(); x++ {
		for v := 0; v < r.M(); v++ {
			r[v][x] = d.D[v][x].Inc(s, n)
		}
	}
	return r
}
