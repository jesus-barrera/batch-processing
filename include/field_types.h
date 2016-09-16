#ifndef _FIELD_TYPES_H_
#define _FIELD_TYPES_H_

#include <vector>
#include <form.h>

typedef std::vector<int> values_list;

extern FIELDTYPE *unique_num_type;

void initFieldTypes();
void endFieldTypes();

#endif
