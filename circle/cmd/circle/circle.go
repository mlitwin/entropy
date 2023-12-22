package main

import (
	"fmt"
	"states"
)

const space = 200
const speeds = 50

const resolution = 4
const precision = 200

func timespeed(a int, b int) int {
	if a < b {
		return 1
	}
	if b < a {
		return -1
	}
	return 0
}

func progress(fraction *int, current int, total int) {
	f := current * 100 / total
	if f != *fraction {
		fmt.Println(f)
	}
	*fraction = f
}

func main() {
	u := states.MakeUniverse(speeds, space)
	//den := states.MakeDistribution(10, 50)
	den := states.MakeDistributionSet(resolution, precision)
	//fmt.Println(ds)

	prog := 0
	for t := 0; t < space; t++ {
		progress(&prog, t, 2*space)

		d := u.Density()
		den.Inc(d[:], 1)
		//fmt.Println(p, d)

		u.Advance()
	}

	var tv = states.NewMatrix[int](resolution, precision)

	for t := 0; t < space; t++ {
		progress(&prog, t+space, 2*space)

		d := u.Density()
		p := den.Val(d[:])
		//fmt.Println(p, d)

		u.Advance()
		d1 := u.Density()
		p2 := den.Val(d1[:])
		tv.ForEach(func(i, j int) {
			tv[i][j] += timespeed(p[i][j], p2[i][j])

		})
	}

	//fmt.Println(tv, den.D.ShannonEntropy())
	trend := 0
	tv.ForEach(func(i int, j int) {
		d := tv[i][j]
		trend += d
		if d != 0 {
			const speeds = 50
			r := float32(j+1) / float32(speeds)
			fmt.Println(i, j, r, d)
		}
	})

	fmt.Println("trend", trend)

}
