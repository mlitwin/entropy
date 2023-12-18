package states

/*
Chaos Umpire sits,
And by decision more imbroiles the fray
By which he Reigns: next him high Arbiter
Chance governs all. Into this wilde Abyss,
The Womb of nature and perhaps her Grave,
Of neither Sea, nor Shore, nor Air, nor Fire,
But all these in thir pregnant causes mixt
Confus'dly, and which thus must ever fight,
Unless th' Almighty Maker them ordain
His dark materials to create more Worlds,
Into this wild Abyss the warie fiend
Stood on the brink of Hell and look'd a while,
Pondering his Voyage: for no narrow frith
He had to cross.
*/

import (
	"math/rand"
)

const D = 3

const plenum = 2
const precision = 5

type Matrix [][]int

func (m *Matrix) M() int {
	return len(*m)
}
func (m *Matrix) N() int {
	return len((*m)[0])
}

type Vector = []int
type Measurement = []int

func NewMatrix(m int, n int) Matrix {
	r := make([][]int, m)
	for j := 0; j < m; j++ {
		r[j] = make([]int, n)
	}

	return r
}

func mod(x int, n int) int {

	return int((x%n + n) % n)
}

type Universe struct {
	M             int
	N             int
	parity        int
	particleCount int
	genesis       Matrix
	blue          Matrix
	green         Matrix
}

func makeDarkMaterials(m *Matrix) (count int) {
	for v := 0; v < m.M(); v++ {
		for x := 0; x < m.N(); x++ {
			n := rand.Intn(plenum)
			(*m)[v][x] = n
			count += n
		}
	}
	return
}

func MakeUniverse(M int, N int) *Universe {
	universe := Universe{M, N, 1, 0, NewMatrix(M, N), NewMatrix(M, N), NewMatrix(M, N)}
	universe.particleCount = makeDarkMaterials(&universe.genesis)
	universe.blue = universe.genesis
	return &universe
}

func (u *Universe) NowAndThen() (*Matrix, *Matrix) {
	if u.parity == 1 {
		return &u.blue, &u.green
	} else {
		return &u.green, &u.blue
	}
}

func nextPosition(x, v, M, N int) int {
	return mod(x+(v-M/2), N)
}

func (u *Universe) Advance() {
	now, then := u.NowAndThen()

	for v := 0; v < u.M; v++ {
		for x := 0; x < u.N; x++ {
			x1 := nextPosition(x, v, u.M, u.N)
			(*then)[v][x1] = (*now)[v][x]
			(*now)[v][x] = 0
		}
	}
	u.parity *= -1
}

func (u *Universe) Density() Vector {
	vec := make(Vector, u.N)
	now, _ := u.NowAndThen()
	for x := 0; x < u.N; x++ {
		for v := 0; v < u.M; v++ {
			vec[x] += (*now)[v][x]
		}
	}
	return vec
}
