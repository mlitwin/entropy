package states

import (
	"testing"
)

func TestMakeDensity(t *testing.T) {
	MakeDensity()
}

func TestBasic(t *testing.T) {
	d := MakeDensity()
	h := make([]int, 10)
	k := d.Val(h)

	if 0 != k {
		t.Fatal(k)
	}

	d.Inc(h, 1)
	k = d.Val(h)
	if 1 != k {
		t.Fatal(k)
	}

	h[1] = 33

	k = d.Val(h)

	if 0 != k {
		t.Fatal(k)
	}

	d.Inc(h, 1)
	k = d.Val(h)
	if 1 != k {
		t.Fatal(k)
	}
}
