package states

type Matrix[T any] [][]T

func (m *Matrix[T]) M() int {
	return len(*m)
}
func (m *Matrix[T]) N() int {
	return len((*m)[0])
}

type Vector = []int
type Measurement = []int

func NewMatrix[T any](m int, n int) Matrix[T] {
	r := make([][]T, m)
	for j := 0; j < m; j++ {
		r[j] = make([]T, n)
	}

	return r
}

func (m *Matrix[T]) ForEach(f func(int, int)) {
	for j := 0; j < m.N(); j++ {
		for i := 0; i < m.M(); i++ {
			f(i, j)
		}
	}
}

func Mod(x int, n int) int {

	return int((x%n + n) % n)
}
