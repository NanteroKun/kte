// MySplitterWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MySplitterWnd.h"


// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWnd)

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CMySplitterWnd::CMySplitterWnd()
		:
	m_nHidedCol(-1),
	m_nHidedRow(-1),
	m_nHidedID(-1)
{
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CMySplitterWnd::~CMySplitterWnd()
{
}

//���b�Z�[�W�}�b�v�J�n
BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
//���b�Z�[�W�}�b�v�I��

//////////////////////////////////////////////////
//�E�B���h�E�X�^�C���̕ύX���s��
//////////////////////////////////////////////////
BOOL CMySplitterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style &= WS_CLIPCHILDREN;
	return CSplitterWnd::PreCreateWindow(cs);
}

//////////////////////////////////////////////////
//���N���b�N��
//////////////////////////////////////////////////
void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//���N���b�N�����������Ȃ����Ƃŕ����E�B���h�E�̈ړ��𐧌�����
	CSplitterWnd::OnLButtonDown(nFlags, point);
}
//////////////////////////////////////////////////
//���}�E�X�{�^����������
//////////////////////////////////////////////////
void CMySplitterWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CSplitterWnd::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////
//�}�E�X�ړ���
//////////////////////////////////////////////////
void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//�}�E�X�ړ������m���Ȃ����ƂŃ}�E�X�A�C�R���̕ύX��}���ł���
	CSplitterWnd::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////
//���\������
//////////////////////////////////////////////////
void CMySplitterWnd::ShowColumn()
{
   ASSERT_VALID(this);
   //ASSERT(m_nCols < m_nMaxCols);
   //ASSERT(m_nHidedCol != -1);
	if (m_nCols == -1)
		return;

   int colNew = m_nHidedCol;
   m_nHidedCol = -1;
   int cxNew = m_pColInfo[m_nCols].nCurSize;
   m_nCols++;    // add a column
   ASSERT(m_nCols == m_nMaxCols);

   // fill the hidden column
   int col;
   for (int row = 0; row < m_nRows; row++)
   {
      CWnd* pPaneShow = GetDlgItem(
         AFX_IDW_PANE_FIRST + row * 16 + m_nCols);
      ASSERT(pPaneShow != NULL);
      pPaneShow->ShowWindow(SW_SHOWNA);
	  m_nHidedID = -1;	//ID���Z�b�g

      for (col = m_nCols - 2; col >= colNew; col--)
      {
         CWnd* pPane = GetPane(row, col);
         ASSERT(pPane != NULL);
         pPane->SetDlgCtrlID(IdFromRowCol(row, col + 1));
      }

      pPaneShow->SetDlgCtrlID(IdFromRowCol(row, colNew));
   }

   // new panes have been created -- recalculate layout
   for (col = colNew + 1; col < m_nCols; col++)
      m_pColInfo[col].nIdealSize = m_pColInfo[col - 1].nCurSize;
   m_pColInfo[colNew].nIdealSize = cxNew;
   RecalcLayout();
}
//////////////////////////////////////////////////
//����B��
//////////////////////////////////////////////////
void CMySplitterWnd::HideColumn(int colHide)
{
   ASSERT_VALID(this);
   ASSERT(m_nCols > 1);
   ASSERT(colHide < m_nCols);
   ASSERT(m_nHidedCol == -1);
   m_nHidedCol = colHide;

   // if the column has an active window -- change it
   /*
   int rowActive = -1;
   int colActive = -1;
   if (GetActivePane(&rowActive, &colActive) != NULL &&
       colActive == colHide)
   {
      if (++colActive >= m_nCols)
         colActive = 0;
      SetActivePane(rowActive, colActive);
   }*/

   // hide all column panes
   for (int row = 0; row < m_nRows; row++)
   {
      CWnd* pPaneHide = GetPane(row, colHide);
      ASSERT(pPaneHide != NULL);
      pPaneHide->ShowWindow(SW_HIDE);

	  //ID�̃Z�b�g
	  m_nHidedID = AFX_IDW_PANE_FIRST + row * 16 + m_nCols;
      pPaneHide->SetDlgCtrlID(m_nHidedID);

      for (int col = colHide + 1; col < m_nCols; col++)
      {
         CWnd* pPane = GetPane(row, col);
         ASSERT(pPane != NULL);
         pPane->SetDlgCtrlID(IdFromRowCol(row, col - 1));
      }
   }
   m_nCols--;
   m_pColInfo[m_nCols].nCurSize = m_pColInfo[colHide].nCurSize;
   RecalcLayout();
}

//////////////////////////////////////////////////
//��̃E�B���h�E�n���h�����擾����
//���̊֐��͗񂪔�\���ł����Ă��J�E���g�����
//////////////////////////////////////////////////
CWnd* CMySplitterWnd::GetColumnWnd(int col)
{
	ASSERT_VALID(this);

	//�񂪕\������Ă���ꍇ
	if (m_nHidedCol == -1)
		return CSplitterWnd::GetPane(0,col);

	//�񂪕\������Ă��Ȃ��ꍇ�Ŕ�\����ƈ�v���Ă���ꍇ
	if (col == m_nHidedCol)
		return GetDlgItem(m_nHidedID);
	if(col > m_nHidedCol)
		return GetPane(0,col - 1);
	return GetPane(0,col);
}



//////////////////////////////////////////////////
//�s��\��
//////////////////////////////////////////////////
void CMySplitterWnd::ShowRow()
{
	ASSERT_VALID(this);
	//ASSERT(m_nRows < m_nMaxRows);
	//ASSERT(m_nHidedRow != -1);
	if (m_nHidedRow == -1)
		return;

	int rowNew = m_nHidedRow;
	m_nHidedRow = -1;
	int cyNew = m_pRowInfo[m_nRows].nCurSize;
	m_nRows++;	//�s�̒ǉ�
	
	ASSERT(m_nRows == m_nMaxRows);

	//�B����Ă����s�̕\��
	int row;
	for (int col = 0; col<m_nCols; col++)
	{
		//TRACE2("GetDlgItem(%d,%d)\n",m_nRows,col);
		CWnd* pPaneShow = GetDlgItem(
			AFX_IDW_PANE_FIRST + m_nRows * 16 + col);
		ASSERT(pPaneShow != NULL);
		pPaneShow->ShowWindow(SW_SHOWNA);
		m_nHidedID = -1;

		for (row = m_nRows - 2; row >= rowNew; row--)
		{
			CWnd* pPane = GetPane(row,col);
			pPane->SetDlgCtrlID(IdFromRowCol(row+1,col));
		}
		pPaneShow->SetDlgCtrlID(IdFromRowCol(rowNew,col));
	}

	//���C�A�E�g�̍Čv�Z
	for (row = rowNew + 1; row < m_nRows; row++) {
		m_pRowInfo[row].nIdealSize = m_pRowInfo[row - 1].nCurSize;
	}
	m_pRowInfo[rowNew].nIdealSize = cyNew;
	RecalcLayout();
}
//////////////////////////////////////////////////
//�s���B��
//////////////////////////////////////////////////
void CMySplitterWnd::HideRow(int rowHide)
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);
	ASSERT(rowHide < m_nRows);
	ASSERT(m_nHidedRow == -1);
	m_nHidedRow = rowHide;

	//�A�N�e�B�u�E�B���h�E�ł���ΕύX����
	/*
	int rowActive = -1;
	int colActive = -1;
	if (GetActivePane(&rowActive,&colActive) != NULL &&
		rowActive == rowHide)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}*/

	//�S�Ă̗�y�C�����B��
	for (int col = 0; col < m_nCols; col++)
	{
		CWnd* pPaneHide = GetPane(rowHide, col);
		ASSERT(pPaneHide != NULL);
		pPaneHide->ShowWindow(SW_HIDE);
		//TRACE2("SetDlgCtrlID(%d,%d)\n",m_nRows,col);

		//ID�̃Z�b�g
		m_nHidedID = AFX_IDW_PANE_FIRST + m_nRows * 16 + col;
		pPaneHide->SetDlgCtrlID(m_nHidedID);

		for (int row = rowHide + 1; row <m_nRows; row++)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row-1,col));
		}
	}
	m_nRows--;
	m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[rowHide].nCurSize;
	RecalcLayout();
}

//////////////////////////////////////////////////
//�s�̃E�B���h�E�n���h�����擾����
//���̊֐��͗񂪔�\���ł����Ă��J�E���g�����
//////////////////////////////////////////////////
CWnd* CMySplitterWnd::GetRowWnd(int row)
{
	ASSERT_VALID(this);

	//�񂪕\������Ă���ꍇ
	if (m_nHidedRow == -1)
		return CSplitterWnd::GetPane(row,0);

	//�񂪕\������Ă��Ȃ��ꍇ�Ŕ�\����ƈ�v���Ă���ꍇ
	if (row == m_nHidedRow)
		return GetDlgItem(m_nHidedID);
	if(row > m_nHidedRow)
		return GetPane(row - 1,0);
	return GetPane(row,0);
}
