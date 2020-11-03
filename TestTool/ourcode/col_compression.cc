#include <stdio.h>
#include "calc.h"
#include "omp.h"

int nnz(int rowArraysize, int* rowArray, int* rowOffset) {
    int i;
    int nnz = 0;
    for (i = 0; i < rowArraysize; i++) {
        int row = rowArray[i];
        for (int p = rowOffset[row]; p < rowOffset[row + 1]; ++p) {
            nnz++;
        }
    }
    return nnz;
}

void columnCompression(int* columnIndice, const int rowArraySize, const int* rowArray,const int* rowOffset) {

    for (int i = 0; i < rowArraySize; i++) {
        int row = rowOffset[rowArray[i] - 1];
        int row_end = rowOffset[rowArray[i]];
        int j;
        for (j = row; j < row_end - 1; j += 2) {
        	columnIndice[j] = (columnIndice[j] << 16) + columnIndice[j + 1];
       		printf("%d,%d\n",columnIndice[j]>>16,columnIndice[j] & 0xFFFF);
       	}
        if (j == row_end - 1) columnIndice[j] = columnIndice[j] << 16;
        //columnIndice Compression completed
    }
}


void func_4_5(const double* valueSpiceMatrix, const int* rowOffset, const int* columnIndice, double* S,
    double* IC, double* IG, int* rowArray, int rowArraySize)
{
#pragma omp parallel for
    for (int i = 0; i < rowArraySize; ++i)
    {
        int row = rowArray[i];
        for (int p = rowOffset[row]; p < rowOffset[row + 1]; ++p)
        {
            int col = columnIndice[p];
            const int k = p * 2;
            double cond = valueSpiceMatrix[k];
            double cap = valueSpiceMatrix[k + 1];

            IG[row] += cond * S[col];
            IC[row] += cap * S[col];
        }
    }
}

void func_6(const int* rowArray, const int* rowOffset, const int rowArraySize, int* columnIndice, const double* S, const double* valueNormalMatrix, double* Id)
{
    columnCompression(columnIndice, rowArraySize, rowArray, rowOffset);
    for (int i = 0; i < rowArraySize; i++)
    {
        const int node = rowArray[i];
	int j;
        for (j = rowOffset[node]; j < rowOffset[node + 1] - 1; j += 2)
        {

            int col_left = columnIndice[j] >> 16;
            int col_right = columnIndice[j] & 0x0000FFFF;
            Id[node] += valueNormalMatrix[j] * S[col_left] + valueNormalMatrix[j + 1] * S[col_right];
	    printf("j = %d, col_left = %d, col_right = %d, row_end = %d, columnIndice = %d\n", j, col_left, col_right, rowOffset[node + 1], columnIndice[j]);
        }
            if (j == rowOffset[node + 1] - 1)
                Id[node] += valueNormalMatrix[j] * (columnIndice[j] >> 16);
    }

}

void func_7_8(const double* valueSpiceMatrix, const int* rowOffset, const int* columnIndice, double* S, double* R, double* H, const double* D, int* rowArray, int rowArraySize)
{
#pragma omp parallel for
    for (int i = 0; i < rowArraySize; ++i)
    {
        int row = rowArray[i];
        const int k1 = row * 2;
        double current = D[k1];
        double charge = D[k1 + 1];

        for (int p = rowOffset[row]; p < rowOffset[row + 1]; ++p)
        {
            int col = columnIndice[p];
            const int k = p * 2;

            current -= valueSpiceMatrix[k] * S[col];
            charge -= valueSpiceMatrix[k + 1] * S[col];
        }

        R[row] = current;
        H[row] = charge;
    }
}

void func_9(const double* valueSpiceMatrix, const int* rowOffset, double* A, double alpha, int* rowArray, int rowArraySize)
{
#pragma omp parallel for
    for (int i = 0; i < rowArraySize; ++i)
    {
        int row = rowArray[i];

        for (int p = rowOffset[row]; p < rowOffset[row + 1]; ++p)
        {
            const int k = p * 2;
            double cond = valueSpiceMatrix[k];
            double cap = valueSpiceMatrix[k + 1];

            A[p] = cond + alpha * cap;
        }
    }
}

void matrix_calc_taskA(TaskMatrixInfoA** ptr, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        func_6(ptr[i]->rowArray, ptr[i]->rowOffset, ptr[i]->rowArraySize, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->valueNormalMatrix, ptr[i]->Id);
    }

}

void matrix_calc_taskB(TaskMatrixInfoB** ptr, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        func_4_5(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->IC, ptr[i]->IG, ptr[i]->rowArray, ptr[i]->rowArraySize);
        func_7_8(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->R, ptr[i]->H, ptr[i]->D, ptr[i]->rowArray, ptr[i]->rowArraySize);
        func_9(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->A, ptr[i]->alpha, ptr[i]->rowArray, ptr[i]->rowArraySize);
    }
}
