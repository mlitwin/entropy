package states

type sliceMapEntry struct {
	valPtr int
	next   map[int](*sliceMapEntry)
}

func makeSliceMapEntry() *sliceMapEntry {
	var e sliceMapEntry

	e.next = make(map[int](*sliceMapEntry))

	return &e
}

type SliceMap struct {
	values []int
	root   *sliceMapEntry
}

func MakeSliceMap() SliceMap {
	var s SliceMap
	s.root = makeSliceMapEntry()

	return s
}

func (s *SliceMap) ValPtr(v []int) (ret int) {
	cur := s.root
	for i, x := range v {
		if i == len(v)-1 {
			if 0 == cur.valPtr {
				cur.valPtr = len(s.values) + 1
				s.values = append(s.values, 0)
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

func (s *SliceMap) Inc(v []int, n int) (ret int) {
	val := s.ValPtr(v)
	ret = s.values[val]
	s.values[val] += n

	return
}

func (s *SliceMap) Val(v []int) (ret int) {
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
