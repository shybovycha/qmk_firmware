#pragma once

/*
left:

 "rows": ["A7", "A6", "A5", "B3", "B4", "B5"],
 "cols": ["B12", "B14", "B15", "A0", "A1", "A15"]

right:

 "rows": ["A7", "A6", "A5", "B3", "B4", "B5"],
 "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
*/

#define MATRIX_ROW_PINS { A7,  A6,  A5,  B3,  B4, B5  }
#define MATRIX_COL_PINS { B12, B13, B14, B15, A8, A15 }

// #define MASTER_LEFT
#define MASTER_RIGHT
