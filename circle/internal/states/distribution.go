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
		for x < (i+1)*resolution {
			t += s[Mod(x, n)]
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
