package states

import (
	"math"
	"strconv"
)

type State []int

func stateHash(s State) (r string) {

	for _, v := range s {
		vs := strconv.Itoa(v)
		if len(r) > 0 {
			r += ":"
		}
		r += vs
	}

	return
}

type Density struct {
	m map[string]int
	n int
}

func MakeDensity() Density {
	d := Density{}
	d.m = make(map[string]int)

	return d
}
func (d *Density) Val(s State) int {
	h := stateHash(s)
	return d.m[h]
}

func (d *Density) Inc(s State, n int) int {
	h := stateHash(s)
	d.m[h] += n
	d.n += n

	return d.m[h]
}

func (d *Density) ShannonEntropy() (ret float64) {
	for _, p := range d.m {
		ret += -(math.Log(float64(p)) - math.Log(float64(d.n)))
	}
	return
}
