package states

import (
	"testing"
)

func TestMatrix(t *testing.T) {
	m := NewMatrix[int](5, 10)
	m.ForEach(func(i, j int) {
		m[i][j] = i + 1000*j
	})

	m.ForEach(func(i, j int) {
		if m[i][j] != i+1000*j {
			t.Fatal("Failed to set", i, j, m[i][j], i+1000*j)
		}
	})
}
