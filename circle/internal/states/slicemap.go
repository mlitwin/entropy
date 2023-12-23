package states

type sliceMapEntry struct {
	val  *int
	next map[int](*sliceMapEntry)
}

func makeSliceMapEntry() *sliceMapEntry {
	var e sliceMapEntry
	var val int
	e.val = &val
	e.next = make(map[int](*sliceMapEntry))

	return &e
}

type SliceMap struct {
	root *sliceMapEntry
}

func MakeSliceMap() SliceMap {
	var s SliceMap
	s.root = makeSliceMapEntry()

	return s
}

func (s *SliceMap) ValPtr(v []int) (ret *int) {
	cur := s.root
	for i, x := range v {
		if i == len(v)-1 {
			ret = cur.val
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
	ret = *val
	*val += n

	return
}

func (s *SliceMap) Val(v []int) (ret int) {
	cur := s.root
	for i, x := range v {
		if nil == cur {
			break
		}
		if i == len(v)-1 {
			ret = *cur.val
		} else {
			cur = cur.next[x]
		}
	}

	return
}
