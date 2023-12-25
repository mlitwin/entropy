package states

type DensityValue interface{}
type DensityIncrementer[T DensityValue] interface {
	Inc(*T, int)
}

type IntIncrementer struct{}

func (IntIncrementer) Inc(x *int, n int) {
	(*x) += n
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

func (s *SliceMap[T]) ValPtr(v []int) (ret int) {
	cur := s.root
	for i, x := range v {
		if i == len(v)-1 {
			if 0 == cur.valPtr {
				cur.valPtr = len(s.values) + 1
				var zero T
				s.values = append(s.values, zero)
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

func (s *SliceMap[T]) Inc(v []int, n int) (ret T) {
	val := s.ValPtr(v)
	ret = s.values[val]
	s.incrementer.Inc(&(s.values[val]), n)

	return
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
