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

	"states"
	// 	"github.com/mlitwin/goeuler/arith"
	// 	"github.com/mlitwin/goeuler/algo"
	//  "github.com/mlitwin/goeuler/textutil"
)

const N = 10
const M = 40

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
	u := states.MakeUniverse(N, M)
	den := states.MakeDistribution(5, 5)

	for t := 0; t < N; t++ {
		d := u.Density()
		p := den.Inc(d[:], 1)
		fmt.Println(p, d)

		u.Advance()
	}

	tv := 0

	for t := 0; t < N; t++ {
		d := u.Density()
		p := den.Val(d[:])
		fmt.Println(p, d)

		u.Advance()
		d1 := u.Density()
		tv += timespeed(p, den.Val(d1[:]))
	}

	fmt.Println(tv, den.D.ShannonEntropy())

}
