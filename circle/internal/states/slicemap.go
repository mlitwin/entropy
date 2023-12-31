package states

type DensityValue interface{}
type DensityIncrementer[T DensityValue] interface {
	Inc(*T, int)
	Construct(*T, Measurement)
}

type IntIncrementer struct{}

func (IntIncrementer) Inc(x *int, n int) {
	(*x) += n
}

func (IntIncrementer) Construct(*int, Measurement) {
}

type sliceMapEntry struct {
	valPtr int
	next   map[int](*sliceMapEntry)
}

func makeSliceMapEntry() *sliceMapEntry {
	var e sliceMapEntry

	e.next = make(map[int](*sliceMapEntry))

	return &e
}

type SliceMap[T DensityValue] struct {
	incrementer DensityIncrementer[T]
	root        *sliceMapEntry
	values      []T
}

func MakeSliceMap[T DensityValue](incrementer DensityIncrementer[T]) SliceMap[T] {
	var s SliceMap[T]
	s.incrementer = incrementer
	s.root = makeSliceMapEntry()

	return s
}

func MakeSliceMapOfInt() SliceMap[int] {
	return MakeSliceMap[int](new(IntIncrementer))

}

func (s *SliceMap[T]) ValPtr(v []int) (ret int) {
	cur := s.root
	for i, x := range v {
		if i == len(v)-1 {
			if cur.valPtr == 0 {
				cur.valPtr = len(s.values) + 1
				var zero T
				s.values = append(s.values, zero)
				s.incrementer.Construct(&s.values[len(s.values)-1], v)
			}
			ret = cur.valPtr - 1
		} else {
			next := cur.next[x]
			if nil == next {
				next = makeSliceMapEntry()
				cur.next[x] = next
			}
			cur = next
		}
	}

	return
}

func (s *SliceMap[T]) IncPtr(val int, n int) (ret T) {
	ret = s.values[val]
	s.incrementer.Inc(&(s.values[val]), n)

	return
}

func (s *SliceMap[T]) ValFromPtr(val int) (ret T) {
	return s.values[val]
}

func (s *SliceMap[T]) Inc(v []int, n int) T {
	return s.IncPtr(s.ValPtr(v), n)
}

func (s *SliceMap[T]) Val(v []int) (ret T) {
	cur := s.root
	for i, x := range v {
		if nil == cur {
			break
		}
		if i == len(v)-1 {
			ret = s.values[cur.valPtr-1]
		} else {
			cur = cur.next[x]
		}
	}

	return
}
