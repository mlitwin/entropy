package states

import (
	"testing"
)

func TestSliceMap(t *testing.T) {
	var v [10]int
	s := MakeSliceMap[int](new(IntIncrementer))

	r := s.Inc(v[:], 1)

	if 0 != r {
		t.Fatal("Can't set", v)
	}
	r = s.Val(v[:])

	if 1 != r {
		t.Fatal("Can't get", r, v)
	}
	v[1] = 1

	r = s.Inc(v[:], 1)
	if 0 != r {
		t.Fatal("Can't set", v)
	}

	r = s.Val(v[:])

	if 1 != r {
		t.Fatal("Can't get", r, v)
	}

	r = s.Inc(v[:], 1)
	if 1 != r {
		t.Fatal("Can't set", v)
	}

	r = s.Val(v[:])

	if 2 != r {
		t.Fatal("Can't get", r, v)
	}

}
