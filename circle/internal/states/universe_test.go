package states

import (
	"testing"
)

const space = 400
const speeds = 50

const resolution = 1
const precision = 40

func BenchmarkUniverse(b *testing.B) {

	u := MakeUniverse(speeds, space)
	den := MakeDistributionSet(resolution, precision)
	//fmt.Println(ds)

	for t := 0; t < space; t++ {
		d := u.Density()
		den.Inc(d[:], 1)
		u.Advance()
	}

}
