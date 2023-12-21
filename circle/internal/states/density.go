package states

import (
	"math"
	"strconv"
	"strings"
)

type State []int

func (d *Density) stateHash(s State) string {

	var sb strings.Builder

	for _, v := range s {
		vs := strconv.Itoa(v / d.Sensitivity)
		sb.WriteString(":")
		sb.WriteString(vs)
	}

	return sb.String()
}

type Density struct {
	Sensitivity int
	m           map[string]int
	n           int
}

func MakeDensity(sensitivity int) Density {
	d := Density{}
	d.Sensitivity = sensitivity
	d.m = make(map[string]int)

	return d
}
func (d *Density) Val(s State) int {
	h := d.stateHash(s)
	return d.m[h]
}

func (d *Density) Inc(s State, n int) int {
	h := d.stateHash(s)
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
