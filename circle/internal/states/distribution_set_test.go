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
			t.Fatal("Failed to set", i, j, v[i][j])
		}
	})
}

func TestSensitivitySetIncrementer(t *testing.T) {
	si := MakeSensitivitySetIncrementer(1)
	var s sensitivitySet
	var v [3]int

	si.Construct(&s, v[:])
	si.Inc(&s, 1)
	index := s.ptrs[0]
	r := si.s[0].ValFromPtr(index)
	if r != 1 {
		t.Fatal("Failed to Inc", r)
	}
}

func TestMakeDistributionSet(t *testing.T) {
	testDistributionSet(t, 5, 4)
	testDistributionSet(t, 4, 5)
	testDistributionSet(t, 40, 5)
	testDistributionSet(t, 400, 50)
}
