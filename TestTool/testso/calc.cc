
#include "calc.h"

void func_4_5(const double* valueSpiceMatrix, const int* rowOffset, const int* columnIndice, double* S,
                double* IC, double* IG, int* rowArray, int rowArraySize)
{
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

void func_6(const int* rowArray, const int* rowOffset, int rowArraySize, const int* columnIndice, const double* S, const double* valueNormalMatrix, double* Id)
{
    for (int i = 0; i < rowArraySize; i++)
    {
        const int node = rowArray[i];

        for (int j = rowOffset[node]; j < rowOffset[node + 1]; ++j)
        {
            Id[node] += valueNormalMatrix[j] * S[columnIndice[j]];
        }
    }
    
}

void func_7_8(const double* valueSpiceMatrix, const int* rowOffset, const int* columnIndice, double* S, double* R, double* H, const double* D, int* rowArray, int rowArraySize)
{
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
            charge -= valueSpiceMatrix[k] * S[col];
        }

        R[row] = current;
        H[row] = charge;
    }
}

void func_9(const double* valueSpiceMatrix, const int* rowOffset, double* A, double alpha, int* rowArray, int rowArraySize)
{
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
    for (int i = 0; i < size; i++)
    {
        func_6(ptr[i]->rowArray, ptr[i]->rowOffset, ptr[i]->rowArraySize, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->valueNormalMatrix, ptr[i]->Id);
    }
    
}

void matrix_calc_taskB(TaskMatrixInfoB** ptr, int size)
{
    for (int i = 0; i < size; i++)
    {
        func_4_5(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->IC, ptr[i]->IG, ptr[i]->rowArray, ptr[i]->rowArraySize);
        func_7_8(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->columnIndice, ptr[i]->S, ptr[i]->R, ptr[i]->H, ptr[i]->D, ptr[i]->rowArray, ptr[i]->rowArraySize);
        func_9(ptr[i]->valueSpiceMatrix, ptr[i]->rowOffset, ptr[i]->A, ptr[i]->alpha, ptr[i]->rowArray, ptr[i]->rowArraySize);
    }
}
