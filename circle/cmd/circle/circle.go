package main

/*
To whom these most adhere,
Hee rules a moment; Chaos Umpire sits,
And by decision more imbroiles the fray
By which he Reigns: next him high Arbiter
Chance governs all. Into this wilde Abyss, [ 910 ]
The Womb of nature and perhaps her Grave,
Of neither Sea, nor Shore, nor Air, nor Fire,
But all these in thir pregnant causes mixt
Confus'dly, and which thus must ever fight,
Unless th' Almighty Maker them ordain [ 915 ]
His dark materials to create more Worlds,
Into this wild Abyss the warie fiend
Stood on the brink of Hell and look'd a while,
Pondering his Voyage: for no narrow frith
He had to cross.
*/

import (
	"fmt"
	"math/rand"

	"states"

	"github.com/mlitwin/goeuler/arith"
	// 	"github.com/mlitwin/goeuler/arith"
	// 	"github.com/mlitwin/goeuler/algo"
	//  "github.com/mlitwin/goeuler/textutil"
)

const D = 3
const N = 10
const M = 40
const plenum = 2
const precision = 5

type Matrix = arith.Matrix[int]
type Vector = [N]int
type Measurement = [D]int

func mod(x int, n int) int {

	return int((x%n + n) % n)
}

type Universe struct {
	parity        int
	particleCount int
	genesis       Matrix
	blue          Matrix
	green         Matrix
}

func makeDarkMaterials(m *Matrix) (count int) {
	for v := 0; v < M; v++ {
		for x := 0; x < N; x++ {
			n := rand.Intn(plenum)
			(*m)[v][x] = n
			count += n
		}
	}
	return
}

func makeUniverse() *Universe {
	universe := Universe{1, 0, arith.NewMatrix[int](M, N), arith.NewMatrix[int](M, N), arith.NewMatrix[int](M, N)}
	universe.particleCount = makeDarkMaterials(&universe.genesis)
	universe.blue = universe.genesis
	return &universe
}

func (u *Universe) nowAndThen() (*Matrix, *Matrix) {
	if u.parity == 1 {
		return &u.blue, &u.green
	} else {
		return &u.green, &u.blue
	}
}

func nextPosition(x, v int) int {
	return mod(x+(v-M/2), N)
}

func (u *Universe) advance() {
	now, then := u.nowAndThen()

	for v := 0; v < M; v++ {
		for x := 0; x < N; x++ {
			x1 := nextPosition(x, v)
			(*then)[v][x1] = (*now)[v][x]
			(*now)[v][x] = 0
		}
	}
	u.parity *= -1
}

func (u *Universe) density() (vec Vector) {
	now, _ := u.nowAndThen()
	for x := 0; x < N; x++ {
		for v := 0; v < M; v++ {
			vec[x] += (*now)[v][x]
		}
	}
	return
}

func timespeed(a int, b int) int {
	if a < b {
		return 1
	}
	if b < a {
		return -1
	}
	return 0
}

func main() {
	u := makeUniverse()
	den := states.MakeDistribution(5, 5)

	for t := 0; t < N; t++ {
		d := u.density()
		p := den.Inc(d[:], 1)
		fmt.Println(p, d)

		u.advance()
	}

	tv := 0

	for t := 0; t < N; t++ {
		d := u.density()
		p := den.Val(d[:])
		fmt.Println(p, d)

		u.advance()
		d1 := u.density()
		tv += timespeed(p, den.Val(d1[:]))
	}

	fmt.Println(tv, den.D.ShannonEntropy())

}
