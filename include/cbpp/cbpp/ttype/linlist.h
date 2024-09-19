#ifndef CBPP_TTYPE_LINKLIST
#define CBPP_TTYPE_LINKLIST

namespace cbpp {
	template<typename T> struct ListNode {
		ListNode *prev, *next;
		T value;
	};
}

#endif