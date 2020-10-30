
#ifndef _CALC_H_
#define _CALC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int*				rowArray;
	const int*			rowOffset;
	int				rowArraySize;
	const int* 			columnIndice;
	const double*			S;
	const double*			valueNormalMatrix;
	double*				Id;
} TaskMatrixInfoA;

typedef struct {
	const double* 			valueSpiceMatrix;
	const int*			rowOffset;
	const int*			columnIndice;
	
	double*				A;
	double*				S;
	double*				R;
	double*				H;
	const double*			D;
	double*				IC;
	double*				IG;
	double				alpha;
	int*				rowArray;
	int				rowArraySize;
	void*				hdl;
} TaskMatrixInfoB;

void matrix_calc_taskA(TaskMatrixInfoA** calcDataList, int size);
void matrix_calc_taskB(TaskMatrixInfoB** calcDataList, int size);

#ifdef __cplusplus
}
#endif

#endif
