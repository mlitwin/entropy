package states

type Distribution struct {
	Resolution  int
	Sensitivity int
	D           Density
}

func (d *Distribution) MakeSubstate(s State) (sub State) {
	n := len(s) / d.Resolution
	if n*d.Resolution < len(s) {
		n++
	}

	sub = make([]int, n)
	for i := range sub {
		t := 0
		x := i * d.Resolution
		for x < (i+1)*d.Resolution {
			t += s[Mod(x, n)]
			x++
		}
		sub[i] = t
	}
	return
}

func MakeDistribution(resolution int, sensitivity int) Distribution {
	d := Distribution{resolution, sensitivity, MakeDensity(sensitivity)}

	return d
}

func (d *Distribution) Val(sub State) int {
	return d.D.Val(sub)
}

func (d *Distribution) Inc(sub State, n int) int {
	return d.D.Inc(sub, n)
}
