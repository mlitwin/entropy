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
)

const space = 20
const speeds = 40

const resolution = 10
const precision = 50

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
	u := states.MakeUniverse(space, speeds)
	//den := states.MakeDistribution(10, 50)
	den := states.MakeDistributionSet(resolution, precision)
	//fmt.Println(ds)

	for t := 0; t < space; t++ {
		d := u.Density()
		den.Inc(d[:], 1)
		//fmt.Println(p, d)

		u.Advance()
	}

	var tv = states.NewMatrix[int](resolution, precision)

	for t := 0; t < space; t++ {
		d := u.Density()
		p := den.Val(d[:])
		//fmt.Println(p, d)

		u.Advance()
		d1 := u.Density()
		p2 := den.Val(d1[:])
		tv.ForEach(func(i, j int) {
			tv[i][j] = timespeed(p[i][j], p2[i][j])

		})
	}

	//fmt.Println(tv, den.D.ShannonEntropy())
	tv.ForEach(func(i int, j int) {
		d := tv[i][j]
		if d != 0 {
			fmt.Println(i, j, d)
		}
	})

}
