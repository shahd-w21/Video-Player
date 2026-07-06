#pragma once
#include "PhotoList.h"

namespace VideoPlayerc {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class PhotoPlayerForm : public System::Windows::Forms::Form
    {
    public:
        PhotoPlayerForm(void)
        {
            InitializeComponent();
            this->photoList = gcnew PhotoList();
            currentZoomLevel = 1.0f;
            isDragging = false;

            // slideshow defaults
            this->slideshowTimer = gcnew System::Windows::Forms::Timer();
            this->slideshowTimer->Interval = 2000;
            this->slideshowIntervalMs = 2000;
            this->slideshowRunning = false;
            this->slideshowTimer->Tick += gcnew System::EventHandler(this, &PhotoPlayerForm::OnSlideshowTick);
        }

    protected:
        ~PhotoPlayerForm()
        {
            if (components)
            {
                delete components;
            }
        }
        // keep OnResize at protected accessibility to match Form::OnResize
        virtual void OnResize(System::EventArgs^ e) override;

    private:
        System::ComponentModel::Container^ components;
        PhotoList^ photoList;
        float currentZoomLevel;
        bool isDragging;
        Point lastMousePos;
        Point imageOffset;
        Image^ originalImage;
        System::Drawing::Size originalBoxSize;

        // UI Components
        System::Windows::Forms::PictureBox^ pictureBoxPhoto;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;
        System::Windows::Forms::Panel^ panelControls;
        System::Windows::Forms::Button^ btnPrevious;
        System::Windows::Forms::Button^ btnNext;
        System::Windows::Forms::Button^ btnZoomIn;
        System::Windows::Forms::Button^ btnZoomOut;
        System::Windows::Forms::Button^ btnZoomReset;
        System::Windows::Forms::Button^ btnRotateLeft;
        System::Windows::Forms::Button^ btnRotateRight;
        System::Windows::Forms::Button^ btnAddPhotos;
        System::Windows::Forms::Button^ btnAddAlbum;
        System::Windows::Forms::Button^ btnRemove;
        System::Windows::Forms::Button^ btnShuffle;
        System::Windows::Forms::Button^ btnSave;
        System::Windows::Forms::Button^ btnLoad;
        System::Windows::Forms::Button^ btnClear;
        System::Windows::Forms::Button^ btnSortAZ;
        System::Windows::Forms::Button^ btnSortTime;
        System::Windows::Forms::Button^ btnSearch;
        // newly added for user requests
        System::Windows::Forms::Button^ btnSlideshow;
        System::Windows::Forms::Button^ btnSpeed;

        System::Windows::Forms::ListBox^ listBoxPhotos;
        System::Windows::Forms::TextBox^ txtSearch;
        System::Windows::Forms::Label^ lblPhotoName;
        System::Windows::Forms::Label^ lblPhotoInfo;
        System::Windows::Forms::Label^ lblZoom;
        System::Windows::Forms::TrackBar^ trackBarZoom;

        // slideshow members
        System::Windows::Forms::Timer^ slideshowTimer;
        bool slideshowRunning;
        int slideshowIntervalMs;

        void InitializeComponent(void)
        {
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->pictureBoxPhoto = (gcnew System::Windows::Forms::PictureBox());
            this->panelControls = (gcnew System::Windows::Forms::Panel());
            this->btnPrevious = (gcnew System::Windows::Forms::Button());
            this->btnNext = (gcnew System::Windows::Forms::Button());
            this->btnZoomIn = (gcnew System::Windows::Forms::Button());
            this->btnZoomOut = (gcnew System::Windows::Forms::Button());
            this->btnZoomReset = (gcnew System::Windows::Forms::Button());
            this->btnRotateLeft = (gcnew System::Windows::Forms::Button());
            this->btnRotateRight = (gcnew System::Windows::Forms::Button());
            this->btnAddPhotos = (gcnew System::Windows::Forms::Button());
            this->btnAddAlbum = (gcnew System::Windows::Forms::Button());
            this->btnRemove = (gcnew System::Windows::Forms::Button());
            this->btnShuffle = (gcnew System::Windows::Forms::Button());
            this->btnSave = (gcnew System::Windows::Forms::Button());
            this->btnLoad = (gcnew System::Windows::Forms::Button());
            this->btnClear = (gcnew System::Windows::Forms::Button());
            this->btnSortAZ = (gcnew System::Windows::Forms::Button());
            this->btnSortTime = (gcnew System::Windows::Forms::Button());
            this->btnSearch = (gcnew System::Windows::Forms::Button());
            this->btnSlideshow = (gcnew System::Windows::Forms::Button());
            this->btnSpeed = (gcnew System::Windows::Forms::Button());
            this->listBoxPhotos = (gcnew System::Windows::Forms::ListBox());
            this->txtSearch = (gcnew System::Windows::Forms::TextBox());
            this->lblPhotoName = (gcnew System::Windows::Forms::Label());
            this->lblPhotoInfo = (gcnew System::Windows::Forms::Label());
            this->lblZoom = (gcnew System::Windows::Forms::Label());
            this->trackBarZoom = (gcnew System::Windows::Forms::TrackBar());

            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxPhoto))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarZoom))->BeginInit();
            this->panelControls->SuspendLayout();
            this->SuspendLayout();

            // pictureBoxBackground
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(1400, 800);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            try {
                String^ imgPath = System::IO::Path::Combine(Application::StartupPath, "Images\\menuBackground.jpg");
                if (System::IO::File::Exists(imgPath)) {
                    this->pictureBoxBackground->Image = Image::FromFile(imgPath);
                }
                else {
                    this->pictureBoxBackground->BackColor = System::Drawing::Color::FromArgb(30, 30, 40);
                }
            }
            catch (...) {
                this->pictureBoxBackground->BackColor = System::Drawing::Color::FromArgb(30, 30, 40);
            }

            // pictureBoxPhoto
            this->pictureBoxPhoto->BackColor = System::Drawing::Color::Transparent;
            this->pictureBoxPhoto->Location = System::Drawing::Point(320, 80);
            this->pictureBoxPhoto->Name = L"pictureBoxPhoto";
            this->pictureBoxPhoto->Size = System::Drawing::Size(760, 560);
            this->pictureBoxPhoto->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->pictureBoxPhoto->TabIndex = 1;
            this->pictureBoxPhoto->TabStop = false;
            this->pictureBoxPhoto->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBoxPhoto->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PhotoPlayerForm::pictureBoxPhoto_MouseDown);
            this->pictureBoxPhoto->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &PhotoPlayerForm::pictureBoxPhoto_MouseMove);
            this->pictureBoxPhoto->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PhotoPlayerForm::pictureBoxPhoto_MouseUp);

            // panelControls
            // Match panel color to the button blue background (panelBg)
            this->panelControls->BackColor = System::Drawing::Color::FromArgb(37, 59, 115);
            this->panelControls->Location = System::Drawing::Point(320, 650);
            this->panelControls->Name = L"panelControls";
            this->panelControls->Size = System::Drawing::Size(760, 120);
            this->panelControls->TabIndex = 2;

            // Standard control button defaults used for icon-only buttons
            System::Drawing::Size iconBtnSize = System::Drawing::Size(36, 36);
            // Icon/button background color (user requested navy blue)
            System::Drawing::Color panelBg = System::Drawing::Color::FromArgb(37, 59, 115);

            // btnPrevious
            this->btnPrevious->BackColor = panelBg;
            this->btnPrevious->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnPrevious->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnPrevious->ForeColor = System::Drawing::Color::White;
            this->btnPrevious->Location = System::Drawing::Point(20, 15);
            this->btnPrevious->Name = L"btnPrevious";
            this->btnPrevious->Size = System::Drawing::Size(48, 48); // enlarged
            this->btnPrevious->TabIndex = 0;
            this->btnPrevious->Text = L"";
            this->btnPrevious->FlatAppearance->BorderSize = 0;
            this->btnPrevious->UseVisualStyleBackColor = false;
            this->btnPrevious->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnPrevious_Click);

            // btnNext
            this->btnNext->BackColor = panelBg;
            this->btnNext->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnNext->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnNext->ForeColor = System::Drawing::Color::White;
            this->btnNext->Location = System::Drawing::Point(76, 15); // shifted for larger buttons
            this->btnNext->Name = L"btnNext";
            this->btnNext->Size = System::Drawing::Size(48, 48);
            this->btnNext->TabIndex = 1;
            this->btnNext->Text = L"";
            this->btnNext->FlatAppearance->BorderSize = 0;
            this->btnNext->UseVisualStyleBackColor = false;
            this->btnNext->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnNext_Click);

            // btnZoomIn
            this->btnZoomIn->BackColor = panelBg;
            this->btnZoomIn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnZoomIn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnZoomIn->ForeColor = System::Drawing::Color::White;
            this->btnZoomIn->Location = System::Drawing::Point(132, 15); // shifted
            this->btnZoomIn->Name = L"btnZoomIn";
            this->btnZoomIn->Size = System::Drawing::Size(48, 48);
            this->btnZoomIn->TabIndex = 2;
            this->btnZoomIn->Text = L"";
            this->btnZoomIn->FlatAppearance->BorderSize = 0;
            this->btnZoomIn->UseVisualStyleBackColor = false;
            this->btnZoomIn->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnZoomIn_Click);

            // btnZoomOut
            this->btnZoomOut->BackColor = panelBg;
            this->btnZoomOut->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnZoomOut->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnZoomOut->ForeColor = System::Drawing::Color::White;
            this->btnZoomOut->Location = System::Drawing::Point(188, 15); // shifted
            this->btnZoomOut->Name = L"btnZoomOut";
            this->btnZoomOut->Size = System::Drawing::Size(48, 48);
            this->btnZoomOut->TabIndex = 3;
            this->btnZoomOut->Text = L"";
            this->btnZoomOut->FlatAppearance->BorderSize = 0;
            this->btnZoomOut->UseVisualStyleBackColor = false;
            this->btnZoomOut->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnZoomOut_Click);

            // btnZoomReset (kept small)
            this->btnZoomReset->BackColor = panelBg;
            this->btnZoomReset->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnZoomReset->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnZoomReset->ForeColor = System::Drawing::Color::White;
            this->btnZoomReset->Location = System::Drawing::Point(244, 15); // shifted
            this->btnZoomReset->Name = L"btnZoomReset";
            this->btnZoomReset->Size = System::Drawing::Size(48, 48);
            this->btnZoomReset->TabIndex = 4;
            this->btnZoomReset->Text = L"";
            this->btnZoomReset->FlatAppearance->BorderSize = 0;
            this->btnZoomReset->UseVisualStyleBackColor = false;
            this->btnZoomReset->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnZoomReset_Click);

            // btnRotateLeft
            this->btnRotateLeft->BackColor = panelBg;
            this->btnRotateLeft->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnRotateLeft->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnRotateLeft->ForeColor = System::Drawing::Color::White;
            this->btnRotateLeft->Location = System::Drawing::Point(300, 15); // shifted
            this->btnRotateLeft->Name = L"btnRotateLeft";
            this->btnRotateLeft->Size = System::Drawing::Size(48, 48);
            this->btnRotateLeft->TabIndex = 5;
            this->btnRotateLeft->Text = L"";
            this->btnRotateLeft->FlatAppearance->BorderSize = 0;
            this->btnRotateLeft->UseVisualStyleBackColor = false;
            this->btnRotateLeft->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnRotateLeft_Click);

            // btnRotateRight
            this->btnRotateRight->BackColor = panelBg;
            this->btnRotateRight->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnRotateRight->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->btnRotateRight->ForeColor = System::Drawing::Color::White;
            this->btnRotateRight->Location = System::Drawing::Point(356, 15); // shifted
            this->btnRotateRight->Name = L"btnRotateRight";
            this->btnRotateRight->Size = System::Drawing::Size(48, 48);
            this->btnRotateRight->TabIndex = 6;
            this->btnRotateRight->Text = L"";
            this->btnRotateRight->FlatAppearance->BorderSize = 0;
            this->btnRotateRight->UseVisualStyleBackColor = false;
            this->btnRotateRight->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnRotateRight_Click);

            // btnSlideshow (new)
            this->btnSlideshow->BackColor = panelBg;
            this->btnSlideshow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSlideshow->Location = System::Drawing::Point(412, 15); // shifted for larger
            this->btnSlideshow->Name = L"btnSlideshow";
            this->btnSlideshow->Size = System::Drawing::Size(48, 48); // enlarged
            this->btnSlideshow->TabIndex = 7;
            this->btnSlideshow->Text = L"";
            this->btnSlideshow->FlatAppearance->BorderSize = 0;
            this->btnSlideshow->UseVisualStyleBackColor = false;
            this->btnSlideshow->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSlideshow_Click);

            // btnSpeed (new)
            this->btnSpeed->BackColor = panelBg;
            this->btnSpeed->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSpeed->Location = System::Drawing::Point(468, 15); // shifted right
            this->btnSpeed->Name = L"btnSpeed";
            this->btnSpeed->Size = System::Drawing::Size(48, 48);
            this->btnSpeed->TabIndex = 8;
            this->btnSpeed->Text = L"";
            this->btnSpeed->FlatAppearance->BorderSize = 0;
            this->btnSpeed->UseVisualStyleBackColor = false;
            this->btnSpeed->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSpeed_Click);

            // btnShuffle (kept small)
            this->btnShuffle->BackColor = panelBg;
            this->btnShuffle->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnShuffle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnShuffle->ForeColor = System::Drawing::Color::White;
            this->btnShuffle->Location = System::Drawing::Point(524, 15); // shifted right
            this->btnShuffle->Name = L"btnShuffle";
            this->btnShuffle->Size = System::Drawing::Size(48, 48);
            this->btnShuffle->TabIndex = 9;
            this->btnShuffle->Text = L"";
            this->btnShuffle->FlatAppearance->BorderSize = 0;
            this->btnShuffle->UseVisualStyleBackColor = false;
            this->btnShuffle->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnShuffle_Click);

            // trackBarZoom
            this->trackBarZoom->BackColor = System::Drawing::Color::FromArgb(40, 40, 50);
            this->trackBarZoom->Location = System::Drawing::Point(580, 65); // moved to fit enlarged buttons
            this->trackBarZoom->Maximum = 400;
            this->trackBarZoom->Minimum = 10;
            this->trackBarZoom->Name = L"trackBarZoom";
            this->trackBarZoom->Size = System::Drawing::Size(180, 45); // reduced width to fit panel
            this->trackBarZoom->TabIndex = 8;
            this->trackBarZoom->TickFrequency = 50;
            this->trackBarZoom->Value = 100;
            this->trackBarZoom->Scroll += gcnew System::EventHandler(this, &PhotoPlayerForm::trackBarZoom_Scroll);

            // lblZoom
            this->lblZoom->BackColor = System::Drawing::Color::Transparent;
            this->lblZoom->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->lblZoom->ForeColor = System::Drawing::Color::White;
            this->lblZoom->Location = System::Drawing::Point(524, 70); // moved next to buttons
            this->lblZoom->Name = L"lblZoom";
            this->lblZoom->Size = System::Drawing::Size(120, 25);
            this->lblZoom->TabIndex = 9;
            this->lblZoom->Text = L"Zoom: 100%";
            this->lblZoom->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

            // add controls to panelControls (order matters visually)
            this->panelControls->Controls->Add(this->btnPrevious);
            this->panelControls->Controls->Add(this->btnNext);
            this->panelControls->Controls->Add(this->btnZoomIn);
            this->panelControls->Controls->Add(this->btnZoomOut);
            this->panelControls->Controls->Add(this->btnZoomReset);
            this->panelControls->Controls->Add(this->btnRotateLeft);
            this->panelControls->Controls->Add(this->btnRotateRight);
            this->panelControls->Controls->Add(this->btnSlideshow);
            this->panelControls->Controls->Add(this->btnSpeed);
            this->panelControls->Controls->Add(this->btnShuffle);
            this->panelControls->Controls->Add(this->trackBarZoom);
            this->panelControls->Controls->Add(this->lblZoom);

            // replace text buttons with image buttons where possible using user's image folder
            try {
                // Use application startup path so images work regardless of developer machine
                String^ base = System::IO::Path::Combine(Application::StartupPath, "Images\\");

                String^ p;

                // Next and Previous
                p = System::IO::Path::Combine(base, "next.png");
                if (System::IO::File::Exists(p)) {
                    this->btnNext->BackgroundImage = Image::FromFile(p);
                    this->btnNext->BackgroundImageLayout = ImageLayout::Zoom;
                }

                p = System::IO::Path::Combine(base, "previous.png");
                if (System::IO::File::Exists(p)) {
                    this->btnPrevious->BackgroundImage = Image::FromFile(p);
                    this->btnPrevious->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Zoom icons (if you have specific images)
                p = System::IO::Path::Combine(base, "zoominbtn.png");
                if (System::IO::File::Exists(p)) {
                    this->btnZoomIn->BackgroundImage = Image::FromFile(p);
                    this->btnZoomIn->BackgroundImageLayout = ImageLayout::Zoom;
                }

                p = System::IO::Path::Combine(base, "zoomoutbtn.png");
                if (System::IO::File::Exists(p)) {
                    this->btnZoomOut->BackgroundImage = Image::FromFile(p);
                    this->btnZoomOut->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Rotate
                // NOTE: user requested these reversed — left gets rotate2, right gets rotate
                p = System::IO::Path::Combine(base, "rotate2.png");
                if (System::IO::File::Exists(p)) {
                    this->btnRotateLeft->BackgroundImage = Image::FromFile(p);
                    this->btnRotateLeft->BackgroundImageLayout = ImageLayout::Zoom;
                }

                p = System::IO::Path::Combine(base, "rotate.png");
                if (System::IO::File::Exists(p)) {
                    this->btnRotateRight->BackgroundImage = Image::FromFile(p);
                    this->btnRotateRight->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Upload / Add Photos
                p = System::IO::Path::Combine(base, "upload (2).png");
                if (System::IO::File::Exists(p)) {
                    this->btnAddPhotos->BackgroundImage = Image::FromFile(p);
                    this->btnAddPhotos->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnAddPhotos->Size = System::Drawing::Size(48,48);
                }

                // Delete / Remove
                p = System::IO::Path::Combine(base, "deletebtn.png");
                if (System::IO::File::Exists(p)) {
                    this->btnRemove->BackgroundImage = Image::FromFile(p);
                    this->btnRemove->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Play / Slideshow
                p = System::IO::Path::Combine(base, "playbtn.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSlideshow->BackgroundImage = Image::FromFile(p);
                    this->btnSlideshow->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Speed
                p = System::IO::Path::Combine(base, "speed.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSpeed->BackgroundImage = Image::FromFile(p);
                    this->btnSpeed->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Shuffle
                p = System::IO::Path::Combine(base, "shuffle.png");
                if (System::IO::File::Exists(p)) {
                    this->btnShuffle->BackgroundImage = Image::FromFile(p);
                    this->btnShuffle->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Search icon
                p = System::IO::Path::Combine(base, "search.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSearch->BackgroundImage = Image::FromFile(p);
                    this->btnSearch->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Sort A-Z (generic sort icon)
                p = System::IO::Path::Combine(base, "sort.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSortAZ->BackgroundImage = Image::FromFile(p);
                    this->btnSortAZ->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Album (use album.png for Add Album)
                p = System::IO::Path::Combine(base, "album.png");
                if (System::IO::File::Exists(p)) {
                    this->btnAddAlbum->BackgroundImage = Image::FromFile(p);
                    this->btnAddAlbum->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnAddAlbum->Size = System::Drawing::Size(48,48);
                }

                // Clear button
                p = System::IO::Path::Combine(base, "clear.png");
                if (System::IO::File::Exists(p)) {
                    this->btnClear->BackgroundImage = Image::FromFile(p);
                    this->btnClear->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnClear->Text = L"";
                    this->btnClear->Size = System::Drawing::Size(48,48);
                }

                // Save button icon
                p = System::IO::Path::Combine(base, "save.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSave->BackgroundImage = Image::FromFile(p);
                    this->btnSave->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnSave->Text = L"";
                    this->btnSave->Size = System::Drawing::Size(48,36);
                }

                // Reset icon for zoom reset
                p = System::IO::Path::Combine(base, "reset.png");
                if (System::IO::File::Exists(p)) {
                    this->btnZoomReset->BackgroundImage = Image::FromFile(p);
                    this->btnZoomReset->BackgroundImageLayout = ImageLayout::Zoom;
                }

                // Load button icon
                p = System::IO::Path::Combine(base, "load.png");
                if (System::IO::File::Exists(p)) {
                    this->btnLoad->BackgroundImage = Image::FromFile(p);
                    this->btnLoad->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnLoad->Text = L"";
                    this->btnLoad->Size = System::Drawing::Size(85,35);
                }

                // Sort by time explicit image
                p = System::IO::Path::Combine(base, "sort by time.png");
                if (System::IO::File::Exists(p)) {
                    this->btnSortTime->BackgroundImage = Image::FromFile(p);
                    this->btnSortTime->BackgroundImageLayout = ImageLayout::Zoom;
                    this->btnSortTime->Text = L"";
                    this->btnSortTime->Size = System::Drawing::Size(85,35);
                }

                // Ensure all icon buttons have no text and border, and same panel background so only icon shows
                array<Button^>^ imgButtons = gcnew array<Button^>(20) {
                    this->btnPrevious, this->btnNext, this->btnZoomIn, this->btnZoomOut,
                    this->btnRotateLeft, this->btnRotateRight, this->btnAddPhotos, this->btnRemove,
                    this->btnSlideshow, this->btnSpeed, this->btnSearch, this->btnSortAZ,
                    this->btnAddAlbum, this->btnClear, this->btnSave, this->btnLoad,
                    this->btnShuffle, this->btnSortTime, this->btnZoomReset, this->btnSearch
                };
                for each (Button^ b in imgButtons) {
                    if (b == nullptr) continue;
                    b->Text = L"";
                    b->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                    b->FlatAppearance->BorderSize = 0;
                    if (b->BackgroundImage == nullptr) {
                        // give a default small appearance even if image missing
                        b->Size = System::Drawing::Size(36,36);
                    }
                    // make the button visually blend
                    if (b->BackColor == System::Drawing::Color::Transparent) {
                        // keep transparent
                    } else {
                        b->BackColor = panelBg;
                    }
                }

                // User requested explicit rotate icons - use exact full paths if present
                try {
                    // swap: left gets rotate2, right gets rotate (use startup Images folder)
                    String^ r1 = System::IO::Path::Combine(base, "rotate2.png");
                    String^ r2 = System::IO::Path::Combine(base, "rotate.png");
                    if (System::IO::File::Exists(r1)) {
                        this->btnRotateLeft->BackgroundImage = Image::FromFile(r1);
                        this->btnRotateLeft->BackgroundImageLayout = ImageLayout::Zoom;
                        this->btnRotateLeft->Text = L"";
                        this->btnRotateLeft->FlatAppearance->BorderSize = 0;
                        this->btnRotateLeft->UseVisualStyleBackColor = false;
                    }
                    if (System::IO::File::Exists(r2)) {
                        this->btnRotateRight->BackgroundImage = Image::FromFile(r2);
                        this->btnRotateRight->BackgroundImageLayout = ImageLayout::Zoom;
                        this->btnRotateRight->Text = L"";
                        this->btnRotateRight->FlatAppearance->BorderSize = 0;
                        this->btnRotateRight->UseVisualStyleBackColor = false;
                    }
                } catch(...) {}
            } catch(...) { }

            // Ensure all Button controls use the requested icon/button background color (panelBg)
            try {
                // Top-level buttons
                for each (Control^ ctl in this->Controls) {
                    Button^ b = dynamic_cast<Button^>(ctl);
                    if (b != nullptr) {
                        b->BackColor = panelBg;
                        b->ForeColor = System::Drawing::Color::White;
                        b->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                        b->FlatAppearance->BorderSize = 0;
                        b->UseVisualStyleBackColor = false;
                    }
                }

                // Buttons inside panelControls
                for each (Control^ ctl in this->panelControls->Controls) {
                    Button^ b = dynamic_cast<Button^>(ctl);
                    if (b != nullptr) {
                        b->BackColor = panelBg;
                        b->ForeColor = System::Drawing::Color::White;
                        b->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                        b->FlatAppearance->BorderSize = 0;
                        b->UseVisualStyleBackColor = false;
                    }
                }
            } catch(...) { }

            // Fallback: ensure slideshow/play button is visible even if image missing
            try {
                if (this->btnSlideshow != nullptr && this->btnSlideshow->BackgroundImage == nullptr) {
                    this->btnSlideshow->Text = L"\u25B6"; // play triangle
                    this->btnSlideshow->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
                    this->btnSlideshow->ForeColor = System::Drawing::Color::White;
                    this->btnSlideshow->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                    this->btnSlideshow->FlatAppearance->BorderSize = 0;
                    this->btnSlideshow->BackColor = panelBg;
                    if (this->btnSlideshow->Size.Width < 40) this->btnSlideshow->Size = System::Drawing::Size(48, 48);
                }
            } catch(...) {}

            // listBoxPhotos
            this->listBoxPhotos->BackColor = System::Drawing::Color::FromArgb(50, 50, 60);
            this->listBoxPhotos->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->listBoxPhotos->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->listBoxPhotos->ForeColor = System::Drawing::Color::White;
            this->listBoxPhotos->FormattingEnabled = true;
            this->listBoxPhotos->ItemHeight = 17;
            this->listBoxPhotos->Location = System::Drawing::Point(20, 180);
            this->listBoxPhotos->Name = L"listBoxPhotos";
            this->listBoxPhotos->Size = System::Drawing::Size(280, 460);
            this->listBoxPhotos->TabIndex = 3;
            this->listBoxPhotos->DoubleClick += gcnew System::EventHandler(this, &PhotoPlayerForm::listBoxPhotos_DoubleClick);

            // txtSearch
            this->txtSearch->BackColor = System::Drawing::Color::FromArgb(60, 60, 70);
            this->txtSearch->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->txtSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
            this->txtSearch->ForeColor = System::Drawing::Color::White;
            // leave room for search icon
            this->txtSearch->Location = System::Drawing::Point(56, 140);
            this->txtSearch->Name = L"txtSearch";
            this->txtSearch->Size = System::Drawing::Size(244, 27);
            this->txtSearch->TabIndex = 4;
            this->txtSearch->Text = L"";
            this->txtSearch->ForeColor = System::Drawing::Color::White;
            this->txtSearch->Enter += gcnew System::EventHandler(this, &PhotoPlayerForm::txtSearch_Enter);
            this->txtSearch->Leave += gcnew System::EventHandler(this, &PhotoPlayerForm::txtSearch_Leave);
            this->txtSearch->TextChanged += gcnew System::EventHandler(this, &PhotoPlayerForm::txtSearch_TextChanged);

            // btnAddPhotos (upload)
            this->btnAddPhotos->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnAddPhotos->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnAddPhotos->ForeColor = System::Drawing::Color::White;
            this->btnAddPhotos->Location = System::Drawing::Point(20, 20);
            this->btnAddPhotos->Name = L"btnAddPhotos";
            // if an image was found above we already shrunk it; otherwise keep original size
            if (this->btnAddPhotos->Size.Width == 0) this->btnAddPhotos->Size = System::Drawing::Size(130, 45);
            this->btnAddPhotos->TabIndex = 5;
            this->btnAddPhotos->Text = L"";
            this->btnAddPhotos->FlatAppearance->BorderSize = 0;
            this->btnAddPhotos->UseVisualStyleBackColor = false;
            this->btnAddPhotos->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnAddPhotos_Click);

            // btnSearch (icon button)
            this->btnSearch->Location = System::Drawing::Point(20, 140);
            this->btnSearch->Size = System::Drawing::Size(28, 28);
            this->btnSearch->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSearch->FlatAppearance->BorderSize = 0;
            this->btnSearch->TabIndex = 100;
            this->btnSearch->TabStop = false;
            this->btnSearch->BackColor = System::Drawing::Color::Transparent;
            this->btnSearch->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
            this->btnSearch->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSearch_Click);

            // btnAddAlbum
            this->btnAddAlbum->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            this->btnAddAlbum->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnAddAlbum->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnAddAlbum->ForeColor = System::Drawing::Color::White;
            this->btnAddAlbum->Location = System::Drawing::Point(160, 20);
            this->btnAddAlbum->Name = L"btnAddAlbum";
            this->btnAddAlbum->Size = System::Drawing::Size(140, 45);
            this->btnAddAlbum->TabIndex = 6;
            this->btnAddAlbum->Text = L"";
            this->btnAddAlbum->FlatAppearance->BorderSize = 0;
            this->btnAddAlbum->UseVisualStyleBackColor = false;
            this->btnAddAlbum->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnAddAlbum_Click);

            // btnRemove
            this->btnRemove->BackColor = panelBg;
            this->btnRemove->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnRemove->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnRemove->ForeColor = System::Drawing::Color::White;
            this->btnRemove->Location = System::Drawing::Point(20, 75);
            this->btnRemove->Name = L"btnRemove";
            this->btnRemove->Size = System::Drawing::Size(48, 48);
            this->btnRemove->TabIndex = 7;
            this->btnRemove->Text = L"";
            this->btnRemove->FlatAppearance->BorderSize = 0;
            this->btnRemove->UseVisualStyleBackColor = false;
            this->btnRemove->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnRemove_Click);

            // btnClear
            this->btnClear->BackColor = System::Drawing::Color::FromArgb(192, 57, 43);
            this->btnClear->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnClear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnClear->ForeColor = System::Drawing::Color::White;
            this->btnClear->Location = System::Drawing::Point(120, 75);
            this->btnClear->Name = L"btnClear";
            this->btnClear->Size = System::Drawing::Size(85, 45);
            this->btnClear->TabIndex = 8;
            this->btnClear->Text = L"";
            this->btnClear->FlatAppearance->BorderSize = 0;
            this->btnClear->UseVisualStyleBackColor = false;
            this->btnClear->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnClear_Click);

            // btnSave
            this->btnSave->BackColor = System::Drawing::Color::FromArgb(41, 128, 185);
            this->btnSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSave->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
            this->btnSave->ForeColor = System::Drawing::Color::White;
            this->btnSave->Location = System::Drawing::Point(20, 650);
            this->btnSave->Name = L"btnSave";
            this->btnSave->Size = System::Drawing::Size(85, 35);
            this->btnSave->TabIndex = 9;
            this->btnSave->Text = L"";
            this->btnSave->FlatAppearance->BorderSize = 0;
            this->btnSave->UseVisualStyleBackColor = false;
            this->btnSave->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSave_Click);

            // btnLoad
            this->btnLoad->BackColor = System::Drawing::Color::FromArgb(39, 174, 96);
            this->btnLoad->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnLoad->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
            this->btnLoad->ForeColor = System::Drawing::Color::White;
            this->btnLoad->Location = System::Drawing::Point(115, 650);
            this->btnLoad->Name = L"btnLoad";
            this->btnLoad->Size = System::Drawing::Size(85, 35);
            this->btnLoad->TabIndex = 10;
            this->btnLoad->Text = L"";
            this->btnLoad->FlatAppearance->BorderSize = 0;
            this->btnLoad->UseVisualStyleBackColor = false;
            this->btnLoad->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnLoad_Click);

            // btnSortAZ
            this->btnSortAZ->BackColor = panelBg;
            this->btnSortAZ->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSortAZ->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
            this->btnSortAZ->ForeColor = System::Drawing::Color::White;
            this->btnSortAZ->Location = System::Drawing::Point(20, 695);
            this->btnSortAZ->Name = L"btnSortAZ";
            this->btnSortAZ->Size = System::Drawing::Size(48, 48);
            this->btnSortAZ->TabIndex = 11;
            this->btnSortAZ->Text = L"";
            this->btnSortAZ->FlatAppearance->BorderSize = 0;
            this->btnSortAZ->UseVisualStyleBackColor = false;
            this->btnSortAZ->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSortAZ_Click);

            // btnSortTime
            this->btnSortTime->BackColor = System::Drawing::Color::FromArgb(142, 68, 173);
            this->btnSortTime->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnSortTime->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
            this->btnSortTime->ForeColor = System::Drawing::Color::White;
            this->btnSortTime->Location = System::Drawing::Point(76, 695);
            this->btnSortTime->Name = L"btnSortTime";
            this->btnSortTime->Size = System::Drawing::Size(85, 35);
            this->btnSortTime->TabIndex = 12;
            this->btnSortTime->Text = L"";
            this->btnSortTime->FlatAppearance->BorderSize = 0;
            this->btnSortTime->UseVisualStyleBackColor = false;
            this->btnSortTime->Click += gcnew System::EventHandler(this, &PhotoPlayerForm::btnSortTime_Click);

            // lblPhotoName
            // make header background match the blue control bar
            this->lblPhotoName->BackColor = panelBg;
            this->lblPhotoName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
            this->lblPhotoName->ForeColor = System::Drawing::Color::White;
            this->lblPhotoName->Location = System::Drawing::Point(320, 20);
            this->lblPhotoName->Name = L"lblPhotoName";
            this->lblPhotoName->Size = System::Drawing::Size(760, 30);
            this->lblPhotoName->TabIndex = 13;
            this->lblPhotoName->Text = L"Photo Player - No Photo Loaded";
            this->lblPhotoName->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

            // lblPhotoInfo
            // make header background match the blue control bar
            this->lblPhotoInfo->BackColor = panelBg;
            this->lblPhotoInfo->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->lblPhotoInfo->ForeColor = System::Drawing::Color::LightGray;
            this->lblPhotoInfo->Location = System::Drawing::Point(320, 50);
            this->lblPhotoInfo->Name = L"lblPhotoInfo";
            this->lblPhotoInfo->Size = System::Drawing::Size(760, 25);
            this->lblPhotoInfo->TabIndex = 14;
            this->lblPhotoInfo->Text = L"Load photos to get started";
            this->lblPhotoInfo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

            // PhotoPlayerForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1400, 800);
            // Add background first so it stays behind other controls
            this->Controls->Add(this->pictureBoxBackground);
            this->Controls->Add(this->lblPhotoInfo);
            this->Controls->Add(this->lblPhotoName);
            this->Controls->Add(this->btnSortTime);
            this->Controls->Add(this->btnSortAZ);
            this->Controls->Add(this->btnLoad);
            this->Controls->Add(this->btnSave);
            this->Controls->Add(this->btnClear);
            this->Controls->Add(this->btnRemove);
            this->Controls->Add(this->btnAddAlbum);
            this->Controls->Add(this->btnAddPhotos);
            this->Controls->Add(this->txtSearch);
            this->Controls->Add(this->listBoxPhotos);
            this->Controls->Add(this->panelControls);
            this->Controls->Add(this->pictureBoxPhoto);
            this->Controls->Add(this->btnSearch);

            try { this->pictureBoxBackground->SendToBack(); } catch(...) {}
            
            // Final enforcement: make sure all Buttons use panelBg (after explicit per-button colors were set above)
            try {
                for each (Control^ ctl in this->Controls) {
                    Button^ b = dynamic_cast<Button^>(ctl);
                    if (b != nullptr) {
                        b->BackColor = panelBg;
                        b->ForeColor = System::Drawing::Color::White;
                        b->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                        b->FlatAppearance->BorderSize = 0;
                        b->UseVisualStyleBackColor = false;
                    }
                }

                for each (Control^ ctl in this->panelControls->Controls) {
                    Button^ b = dynamic_cast<Button^>(ctl);
                    if (b != nullptr) {
                        b->BackColor = panelBg;
                        b->ForeColor = System::Drawing::Color::White;
                        b->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                        b->FlatAppearance->BorderSize = 0;
                        b->UseVisualStyleBackColor = false;
                    }
                }
            } catch(...) {}

            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->Name = L"PhotoPlayerForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Photo Player";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxPhoto))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarZoom))->EndInit();
            this->panelControls->ResumeLayout(false);
            this->panelControls->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        // Event Handlers
        System::Void btnAddPhotos_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSearch_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnAddAlbum_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRemove_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnClear_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSave_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnLoad_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSortAZ_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSortTime_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnPrevious_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnNext_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnShuffle_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnZoomIn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnZoomOut_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnZoomReset_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRotateLeft_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRotateRight_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSlideshow_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSpeed_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void listBoxPhotos_DoubleClick(System::Object^ sender, System::EventArgs^ e);
        System::Void txtSearch_Enter(System::Object^ sender, System::EventArgs^ e);
        System::Void txtSearch_Leave(System::Object^ sender, System::EventArgs^ e);
        System::Void txtSearch_TextChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void trackBarZoom_Scroll(System::Object^ sender, System::EventArgs^ e);
        System::Void pictureBoxPhoto_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void pictureBoxPhoto_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void pictureBoxPhoto_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

        // btnSearch_Click is implemented in PhotoPlayerForm.cpp

        // Helper Methods
        void loadPhoto(String^ photoPath);
        void updatePhotoInfo();
        void applyZoom(float zoomLevel);
        // adjust picture box / image aspect similar to video player behavior
        void AdjustPictureBoxAspect();
        void performSearch();
        void selectCurrentInList();
        System::Void OnSlideshowTick(System::Object^ sender, System::EventArgs^ e);
    };
}