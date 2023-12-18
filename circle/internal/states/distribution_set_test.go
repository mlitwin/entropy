package states

import (
	"testing"
)

func testDistributionSet(t *testing.T, resolution int, precision int) {
	d := MakeDistributionSet(resolution, precision)

	var s [10]int

	d.Inc(s[:], 7)
	v := d.Val(s[:])
	v.ForEach(func(i, j int) {
		if v[i][j] != 7 {
			t.Fatal("Failed to set")
		}
	})
}

func TestMakeDistributionSet(t *testing.T) {
	testDistributionSet(t, 5, 4)
	testDistributionSet(t, 4, 5)
	testDistributionSet(t, 40, 5)
	testDistributionSet(t, 400, 50)
}
