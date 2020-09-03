#pragma once

#include "COMMON/OglForCLI.h"


namespace Simple3DObjectsViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainForm の概要
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	private:
		MainForm(void);

	static MainForm^ m_singleton;
	private: System::Windows::Forms::Button^ m_btn_loadobjcts;
	private: System::Windows::Forms::Button^ m_btn_normalize1;
	private: System::Windows::Forms::Button^ m_btn_normalize2;
	private: System::Windows::Forms::Button^ m_btn_alignobjs;
	private: System::Windows::Forms::Button^ m_btn_calcdiff;

	public:
		System::Windows::Forms::CheckBox^ m_cb_obj1edge;
		System::Windows::Forms::CheckBox^ m_cb_obj1surf;
		System::Windows::Forms::CheckBox^ m_cb_obj2surf;
		System::Windows::Forms::CheckBox^ m_cb_obj2edge;
		System::Windows::Forms::CheckBox^ m_cb_diffverts;
		System::Windows::Forms::CheckBox^ m_cb_diffofst;




				 OglForCLI* m_ogl;

	public:
		static MainForm^ GetInst()
		{
			if (m_singleton == nullptr) m_singleton = gcnew MainForm();
			return m_singleton;
		}
		void RedrawMainPanel();

	public:


	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^ m_panel;
	protected:

	protected:

	private:
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_panel = (gcnew System::Windows::Forms::Panel());
			this->m_btn_loadobjcts = (gcnew System::Windows::Forms::Button());
			this->m_btn_normalize1 = (gcnew System::Windows::Forms::Button());
			this->m_btn_normalize2 = (gcnew System::Windows::Forms::Button());
			this->m_btn_alignobjs = (gcnew System::Windows::Forms::Button());
			this->m_btn_calcdiff = (gcnew System::Windows::Forms::Button());
			this->m_cb_obj2surf = (gcnew System::Windows::Forms::CheckBox());
			this->m_cb_obj2edge = (gcnew System::Windows::Forms::CheckBox());
			this->m_cb_obj1edge = (gcnew System::Windows::Forms::CheckBox());
			this->m_cb_obj1surf = (gcnew System::Windows::Forms::CheckBox());
			this->m_cb_diffverts = (gcnew System::Windows::Forms::CheckBox());
			this->m_cb_diffofst = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// m_panel
			// 
			this->m_panel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->m_panel->AutoSize = true;
			this->m_panel->Location = System::Drawing::Point(12, 12);
			this->m_panel->Name = L"m_panel";
			this->m_panel->Size = System::Drawing::Size(684, 534);
			this->m_panel->TabIndex = 0;
			this->m_panel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::m_panel_Paint);
			this->m_panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_panel_MouseDown);
			this->m_panel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_panel_MouseMove);
			this->m_panel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_panel_MouseUp);
			this->m_panel->Resize += gcnew System::EventHandler(this, &MainForm::m_panel_Resize);
			// 
			// m_btn_loadobjcts
			// 
			this->m_btn_loadobjcts->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->m_btn_loadobjcts->Location = System::Drawing::Point(12, 550);
			this->m_btn_loadobjcts->Name = L"m_btn_loadobjcts";
			this->m_btn_loadobjcts->Size = System::Drawing::Size(89, 23);
			this->m_btn_loadobjcts->TabIndex = 1;
			this->m_btn_loadobjcts->Text = L"Load Objects";
			this->m_btn_loadobjcts->UseVisualStyleBackColor = true;
			this->m_btn_loadobjcts->Click += gcnew System::EventHandler(this, &MainForm::m_btn_loadobjcts_Click);
			// 
			// m_btn_normalize1
			// 
			this->m_btn_normalize1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->m_btn_normalize1->Location = System::Drawing::Point(12, 579);
			this->m_btn_normalize1->Name = L"m_btn_normalize1";
			this->m_btn_normalize1->Size = System::Drawing::Size(150, 36);
			this->m_btn_normalize1->TabIndex = 2;
			this->m_btn_normalize1->Text = L"Normalize size\r\n (mean dist from centroid)";
			this->m_btn_normalize1->UseVisualStyleBackColor = true;
			this->m_btn_normalize1->Click += gcnew System::EventHandler(this, &MainForm::m_btn_normalize1_Click);
			// 
			// m_btn_normalize2
			// 
			this->m_btn_normalize2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->m_btn_normalize2->Location = System::Drawing::Point(12, 618);
			this->m_btn_normalize2->Name = L"m_btn_normalize2";
			this->m_btn_normalize2->Size = System::Drawing::Size(150, 36);
			this->m_btn_normalize2->TabIndex = 3;
			this->m_btn_normalize2->Text = L"Normalize size\r\n (dist of farthest 2 verts )";
			this->m_btn_normalize2->UseVisualStyleBackColor = true;
			this->m_btn_normalize2->Click += gcnew System::EventHandler(this, &MainForm::m_btn_normalize2_Click);
			// 
			// m_btn_alignobjs
			// 
			this->m_btn_alignobjs->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->m_btn_alignobjs->Location = System::Drawing::Point(168, 579);
			this->m_btn_alignobjs->Name = L"m_btn_alignobjs";
			this->m_btn_alignobjs->Size = System::Drawing::Size(167, 36);
			this->m_btn_alignobjs->TabIndex = 4;
			this->m_btn_alignobjs->Text = L"Align Objects ICP\r\nmove obj1 to fit to obj2";
			this->m_btn_alignobjs->UseVisualStyleBackColor = true;
			this->m_btn_alignobjs->Click += gcnew System::EventHandler(this, &MainForm::m_btn_alignobjs_Click);
			// 
			// m_btn_calcdiff
			// 
			this->m_btn_calcdiff->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->m_btn_calcdiff->Location = System::Drawing::Point(168, 618);
			this->m_btn_calcdiff->Name = L"m_btn_calcdiff";
			this->m_btn_calcdiff->Size = System::Drawing::Size(167, 36);
			this->m_btn_calcdiff->TabIndex = 5;
			this->m_btn_calcdiff->Text = L"Compute Difference\r\ndist from verts of obj1 to obj2";
			this->m_btn_calcdiff->UseVisualStyleBackColor = true;
			this->m_btn_calcdiff->Click += gcnew System::EventHandler(this, &MainForm::m_btn_calcdiff_Click);
			// 
			// m_cb_obj2surf
			// 
			this->m_cb_obj2surf->AutoSize = true;
			this->m_cb_obj2surf->Checked = true;
			this->m_cb_obj2surf->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_obj2surf->Location = System::Drawing::Point(357, 613);
			this->m_cb_obj2surf->Name = L"m_cb_obj2surf";
			this->m_cb_obj2surf->Size = System::Drawing::Size(69, 16);
			this->m_cb_obj2surf->TabIndex = 6;
			this->m_cb_obj2surf->Text = L"obj2_surf";
			this->m_cb_obj2surf->UseVisualStyleBackColor = true;
			this->m_cb_obj2surf->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_obj2surf_CheckedChanged);
			// 
			// m_cb_obj2edge
			// 
			this->m_cb_obj2edge->AutoSize = true;
			this->m_cb_obj2edge->Checked = true;
			this->m_cb_obj2edge->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_obj2edge->Location = System::Drawing::Point(357, 635);
			this->m_cb_obj2edge->Name = L"m_cb_obj2edge";
			this->m_cb_obj2edge->Size = System::Drawing::Size(73, 16);
			this->m_cb_obj2edge->TabIndex = 7;
			this->m_cb_obj2edge->Text = L"obj2_edge";
			this->m_cb_obj2edge->UseVisualStyleBackColor = true;
			this->m_cb_obj2edge->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_obj2edge_CheckedChanged);
			// 
			// m_cb_obj1edge
			// 
			this->m_cb_obj1edge->AutoSize = true;
			this->m_cb_obj1edge->Checked = true;
			this->m_cb_obj1edge->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_obj1edge->Location = System::Drawing::Point(357, 591);
			this->m_cb_obj1edge->Name = L"m_cb_obj1edge";
			this->m_cb_obj1edge->Size = System::Drawing::Size(73, 16);
			this->m_cb_obj1edge->TabIndex = 9;
			this->m_cb_obj1edge->Text = L"obj1_edge";
			this->m_cb_obj1edge->UseVisualStyleBackColor = true;
			this->m_cb_obj1edge->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_obj1edge_CheckedChanged);
			// 
			// m_cb_obj1surf
			// 
			this->m_cb_obj1surf->AutoSize = true;
			this->m_cb_obj1surf->Checked = true;
			this->m_cb_obj1surf->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_obj1surf->Location = System::Drawing::Point(357, 569);
			this->m_cb_obj1surf->Name = L"m_cb_obj1surf";
			this->m_cb_obj1surf->Size = System::Drawing::Size(69, 16);
			this->m_cb_obj1surf->TabIndex = 8;
			this->m_cb_obj1surf->Text = L"obj1_surf";
			this->m_cb_obj1surf->UseVisualStyleBackColor = true;
			this->m_cb_obj1surf->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_obj1surf_CheckedChanged);
			// 
			// m_cb_diffverts
			// 
			this->m_cb_diffverts->AutoSize = true;
			this->m_cb_diffverts->Checked = true;
			this->m_cb_diffverts->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_diffverts->Location = System::Drawing::Point(447, 569);
			this->m_cb_diffverts->Name = L"m_cb_diffverts";
			this->m_cb_diffverts->Size = System::Drawing::Size(71, 16);
			this->m_cb_diffverts->TabIndex = 6;
			this->m_cb_diffverts->Text = L"diff_verts";
			this->m_cb_diffverts->UseVisualStyleBackColor = true;
			this->m_cb_diffverts->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_diffverts_CheckedChanged);
			// 
			// m_cb_diffofst
			// 
			this->m_cb_diffofst->AutoSize = true;
			this->m_cb_diffofst->Checked = true;
			this->m_cb_diffofst->CheckState = System::Windows::Forms::CheckState::Checked;
			this->m_cb_diffofst->Location = System::Drawing::Point(447, 591);
			this->m_cb_diffofst->Name = L"m_cb_diffofst";
			this->m_cb_diffofst->Size = System::Drawing::Size(65, 16);
			this->m_cb_diffofst->TabIndex = 7;
			this->m_cb_diffofst->Text = L"diff_ofst";
			this->m_cb_diffofst->UseVisualStyleBackColor = true;
			this->m_cb_diffofst->CheckedChanged += gcnew System::EventHandler(this, &MainForm::m_cb_diffofst_CheckedChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(708, 661);
			this->Controls->Add(this->m_cb_obj1edge);
			this->Controls->Add(this->m_cb_obj1surf);
			this->Controls->Add(this->m_cb_diffofst);
			this->Controls->Add(this->m_cb_obj2edge);
			this->Controls->Add(this->m_cb_diffverts);
			this->Controls->Add(this->m_cb_obj2surf);
			this->Controls->Add(this->m_btn_calcdiff);
			this->Controls->Add(this->m_btn_alignobjs);
			this->Controls->Add(this->m_btn_normalize2);
			this->Controls->Add(this->m_btn_normalize1);
			this->Controls->Add(this->m_btn_loadobjcts);
			this->Controls->Add(this->m_panel);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyDown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void m_panel_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void m_panel_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void m_panel_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void m_panel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
	private: System::Void m_panel_Resize(System::Object^ sender, System::EventArgs^ e);
	private: System::Void MainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
	private: System::Void m_btn_loadobjcts_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void m_btn_normalize1_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void m_btn_normalize2_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void m_btn_alignobjs_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void m_btn_calcdiff_Click(System::Object^ sender, System::EventArgs^ e) ;
	private: System::Void m_cb_obj1surf_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
	private: System::Void m_cb_obj1edge_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
	private: System::Void m_cb_obj2surf_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
	private: System::Void m_cb_obj2edge_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
	private: System::Void m_cb_diffverts_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
	private: System::Void m_cb_diffofst_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		RedrawMainPanel();
	}
};

	inline void RedrawMainWindow(){ MainForm::GetInst()->RedrawMainPanel(); }
	inline bool IsDrawObj1Surf () { return MainForm::GetInst()->m_cb_obj1surf ->Checked; }
	inline bool IsDrawObj1Edge () { return MainForm::GetInst()->m_cb_obj1edge ->Checked; }
	inline bool IsDrawObj2Surf () { return MainForm::GetInst()->m_cb_obj2surf ->Checked; }
	inline bool IsDrawObj2Edge () { return MainForm::GetInst()->m_cb_obj2edge ->Checked; }
	inline bool IsDrawDiffVerts() { return MainForm::GetInst()->m_cb_diffverts->Checked; }
	inline bool IsDrawDiffOfst () { return MainForm::GetInst()->m_cb_diffofst ->Checked; }



}
