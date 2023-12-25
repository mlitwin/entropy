package states

import (
	"testing"
)

func TestMakeDistribution(t *testing.T) {
	d := MakeDistribution(5, 4)
	var s [10]int

	d.Inc(s[:], 7)

	if d.Val(s[:]) != 7 {
		t.Fatal("Failed to set")
	}
}
