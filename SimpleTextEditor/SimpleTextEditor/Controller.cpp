#include "Header.h"

Controller::Controller(WindowModel* model) {
    m_myModel = model;
}

void Controller::GetKeyFromView(int key) {
    m_myModel->GetKeyFromController(key);
}

void Controller::PutModelNewIdx(size_t new_idx) {
    m_myModel->idx = new_idx;
}

void Controller::GetIdxForMove() {
    m_myModel->NotifyIdxForMove(m_myModel->file_data, m_myModel->idx);
}

void Controller::PutModelFirstLastIdx(size_t first_idx, size_t last_idx) {
    m_myModel->FirstIdxCopyDel = first_idx;
    m_myModel->LastIdxCopyDel = last_idx;
}
void Controller::PutModelReplace(int c) {
    m_myModel->GetSymbolReplace(c);
}