module circle

go 1.21.4

replace states => ./internal/states

require (
	github.com/mlitwin/goeuler/arith v0.0.0-20221008204731-d7065a3ce783
	states v0.0.0-00010101000000-000000000000
)
