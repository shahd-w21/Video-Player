#pragma once
#include "LinkedList.h"

namespace VideoPlayerc {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::IO;
    using namespace System::Drawing;

    public ref class MainForm : public Form
    {
    private:
        System::Windows::Forms::Button^ btnDelete;
        System::Windows::Forms::Button^ btnUpload;
        System::Windows::Forms::Button^ btnPlay;
        System::Windows::Forms::Button^ btnMoreOptions;  
        System::Windows::Forms::Button^ btnBackToMenu;
        System::Windows::Forms::Panel^ videoPanel;
        System::Windows::Forms::ListBox^ listBox2;
        System::Windows::Forms::PictureBox^ pictureBoxBackground;
        System::Windows::Forms::ContextMenuStrip^ moreOptionsMenu;  
        AxWMPLib::AxWindowsMediaPlayer^ mediaPlayer;
        VideoList^ videoList;
        bool isPlaying;
        bool isDraggingSlider;

        // Extra controls (added features from first code)
        System::Windows::Forms::Panel^ controlPanel;
        System::Windows::Forms::Button^ playButton;
        System::Windows::Forms::Button^ pauseButton;
        System::Windows::Forms::Button^ nextButton;
        System::Windows::Forms::Button^ previousButton;
        System::Windows::Forms::Button^ shuffleButton;
        System::Windows::Forms::Button^ fullScreenButton;

        System::Windows::Forms::Button^ loopButton;
        System::Windows::Forms::Button^ skipForwardButton;
        System::Windows::Forms::Button^ skipBackwardButton;
        System::Windows::Forms::Button^ resetButton;
        System::Windows::Forms::TrackBar^ positionTrackBar;
        System::Windows::Forms::TrackBar^ volumeTrackBar;
        System::Windows::Forms::Label^ timeLabel;
        System::Windows::Forms::ComboBox^ speedComboBox;
        System::Windows::Forms::Label^ playlistInfoLabel;
        System::Windows::Forms::PictureBox^ volumeIcon;
        System::Windows::Forms::PictureBox^ speedIcon;
        System::Windows::Forms::Timer^ timer;
        bool isFullscreen;
        // Single loop mode: when true playlist loops (repeat all). Removed 'one' mode.
        bool loopEnabled;
        System::Collections::Generic::Dictionary<String^, System::Drawing::Point>^ originalPositions;
        System::Collections::Generic::Dictionary<String^, System::Drawing::Size>^ originalSizes;
        bool useOriginalFullscreenPositions;
        System::Drawing::Size originalControlPanelSize;
        // store previous window state for fullscreen toggle
        System::Windows::Forms::FormWindowState previousWindowState;
        System::Windows::Forms::FormBorderStyle previousBorderStyle;
        System::Drawing::Rectangle previousBounds;

    public:
        // Constructor: initialize UI, load resources, and set up event handlers
        MainForm(void)
        {
            InitializeComponent();
            // use crosshair cursor for the application at startup
            this->Cursor = Cursors::Cross;
            // Ensure mouse move handlers are always attached; they check isFullscreen before acting
            this->videoPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
            this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::VideoPanel_MouseMove);
            this->mediaPlayer->uiMode = "none";
            videoList = gcnew VideoList();
            isPlaying = false;
            isDraggingSlider = false;
            LoadBackgroundImage();
            LoadButtonImages();
            CreateMoreOptionsMenu();  // NEW - from second code
            PositionListBox();
            PositionButtons();
            PositionMediaPlayer();
            AutoLoadPlaylist();
            isFullscreen = false;
            loopEnabled = false;
            this->CaptureOriginalControlPositions();
            this->useOriginalFullscreenPositions = false;
            // Ensure UI images reflect initial state
            this->UpdateLoopButtonImage();
            // Populate runtime-only items that can confuse the designer if present in InitializeComponent
            try {
                if (this->speedComboBox != nullptr && this->speedComboBox->Items->Count == 0) {
                    this->speedComboBox->Items->Add("0.5x");
                    this->speedComboBox->Items->Add("1.0x");
                    this->speedComboBox->Items->Add("1.25x");
                    this->speedComboBox->Items->Add("1.5x");
                    this->speedComboBox->Items->Add("2.0x");
                }
            } catch (Exception^) { }
        }

    protected:
        // Destructor: cleanup components container
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

        // Called when the form is closing; perform autosave then forward event
        virtual void OnFormClosing(FormClosingEventArgs^ e) override
        {
            AutoSavePlaylist();
            Form::OnFormClosing(e);
        }

        // Called on form resize; reposition major UI elements
        virtual void OnResize(EventArgs^ e) override
        {
            Form::OnResize(e);
            PositionListBox();
            PositionButtons();
            PositionMediaPlayer();
        }
    private: System::ComponentModel::IContainer^ components;
    protected:

    private:




        // InitializeComponent: designer-generated setup for controls and layout
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->btnDelete = (gcnew System::Windows::Forms::Button());
            this->btnUpload = (gcnew System::Windows::Forms::Button());
            this->btnPlay = (gcnew System::Windows::Forms::Button());
            this->btnMoreOptions = (gcnew System::Windows::Forms::Button());
            this->btnBackToMenu = (gcnew System::Windows::Forms::Button());
            this->videoPanel = (gcnew System::Windows::Forms::Panel());
            this->mediaPlayer = (gcnew AxWMPLib::AxWindowsMediaPlayer());
            this->controlPanel = (gcnew System::Windows::Forms::Panel());
            this->positionTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->timeLabel = (gcnew System::Windows::Forms::Label());
            this->playButton = (gcnew System::Windows::Forms::Button());
            this->pauseButton = (gcnew System::Windows::Forms::Button());
            this->previousButton = (gcnew System::Windows::Forms::Button());
            this->nextButton = (gcnew System::Windows::Forms::Button());
            this->resetButton = (gcnew System::Windows::Forms::Button());
            this->shuffleButton = (gcnew System::Windows::Forms::Button());
            this->fullScreenButton = (gcnew System::Windows::Forms::Button());
            this->loopButton = (gcnew System::Windows::Forms::Button());
            this->skipForwardButton = (gcnew System::Windows::Forms::Button());
            this->skipBackwardButton = (gcnew System::Windows::Forms::Button());
            this->volumeIcon = (gcnew System::Windows::Forms::PictureBox());
            this->volumeTrackBar = (gcnew System::Windows::Forms::TrackBar());
            this->speedIcon = (gcnew System::Windows::Forms::PictureBox());
            this->speedComboBox = (gcnew System::Windows::Forms::ComboBox());
            this->playlistInfoLabel = (gcnew System::Windows::Forms::Label());
            this->timer = (gcnew System::Windows::Forms::Timer(this->components));
            this->listBox2 = (gcnew System::Windows::Forms::ListBox());
            this->pictureBoxBackground = (gcnew System::Windows::Forms::PictureBox());
            this->moreOptionsMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->videoPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->BeginInit();
            this->controlPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeIcon))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->speedIcon))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->BeginInit();
            this->SuspendLayout();
            // 
            // btnDelete
            // 
            this->btnDelete->Location = System::Drawing::Point(279, 342);
            this->btnDelete->Name = L"btnDelete";
            this->btnDelete->Size = System::Drawing::Size(68, 54);
            this->btnDelete->TabIndex = 1;
            this->btnDelete->Text = L"Delete";
            this->btnDelete->UseVisualStyleBackColor = true;
            this->btnDelete->Click += gcnew System::EventHandler(this, &MainForm::btnDelete_Click);
            // 
            // btnUpload
            // 
            this->btnUpload->Location = System::Drawing::Point(386, 342);
            this->btnUpload->Name = L"btnUpload";
            this->btnUpload->Size = System::Drawing::Size(70, 63);
            this->btnUpload->TabIndex = 2;
            this->btnUpload->Text = L"Upload";
            this->btnUpload->UseVisualStyleBackColor = true;
            this->btnUpload->Click += gcnew System::EventHandler(this, &MainForm::btnUpload_Click);
            // 
            // btnPlay
            // 
            this->btnPlay->Location = System::Drawing::Point(394, 185);
            this->btnPlay->Name = L"btnPlay";
            this->btnPlay->Size = System::Drawing::Size(67, 73);
            this->btnPlay->TabIndex = 4;
            this->btnPlay->Text = L"Play";
            this->btnPlay->UseVisualStyleBackColor = true;
            this->btnPlay->Click += gcnew System::EventHandler(this, &MainForm::btnPlay_Click);
            // 
            // btnMoreOptions
            // 
            this->btnMoreOptions->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
            this->btnMoreOptions->Location = System::Drawing::Point(500, 112);
            this->btnMoreOptions->Name = L"btnMoreOptions";
            this->btnMoreOptions->Size = System::Drawing::Size(100, 40);
            this->btnMoreOptions->TabIndex = 5;
            // Avoid special Unicode vertical-ellipsis character in designer code to prevent CodeDOM parser errors
            this->btnMoreOptions->Text = L"...";
            this->btnMoreOptions->UseVisualStyleBackColor = true;
            this->btnMoreOptions->Click += gcnew System::EventHandler(this, &MainForm::btnMoreOptions_Click);
            // 
            // btnBackToMenu
            // 
            this->btnBackToMenu->BackColor = System::Drawing::Color::Transparent;
            this->btnBackToMenu->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"btnBackToMenu.BackgroundImage")));
            this->btnBackToMenu->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->btnBackToMenu->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnBackToMenu->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnBackToMenu->ForeColor = System::Drawing::Color::White;
            this->btnBackToMenu->Location = System::Drawing::Point(0, 0);
            this->btnBackToMenu->Name = L"btnBackToMenu";
            this->btnBackToMenu->Size = System::Drawing::Size(97, 40);
            this->btnBackToMenu->TabIndex = 8;
            this->btnBackToMenu->UseVisualStyleBackColor = false;
            this->btnBackToMenu->Click += gcnew System::EventHandler(this, &MainForm::btnBackToMenu_Click);
            // 
            // videoPanel
            // 
            this->videoPanel->BackColor = System::Drawing::Color::Black;
            this->videoPanel->Controls->Add(this->mediaPlayer);
            this->videoPanel->Controls->Add(this->btnBackToMenu);
            this->videoPanel->Location = System::Drawing::Point(50, 16);
            this->videoPanel->Name = L"videoPanel";
            this->videoPanel->Size = System::Drawing::Size(640, 380);
            this->videoPanel->TabIndex = 9;
            this->videoPanel->Visible = false;
            this->videoPanel->Resize += gcnew System::EventHandler(this, &MainForm::VideoPanel_Resize_Handler);
            // 
            // mediaPlayer
            // 
            this->mediaPlayer->Enabled = true;
            this->mediaPlayer->Location = System::Drawing::Point(-3, -36);
            this->mediaPlayer->Name = L"mediaPlayer";
            this->mediaPlayer->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"mediaPlayer.OcxState")));
            this->mediaPlayer->Size = System::Drawing::Size(640, 420);
            this->mediaPlayer->TabIndex = 0;
            this->mediaPlayer->PlayStateChange += gcnew AxWMPLib::_WMPOCXEvents_PlayStateChangeEventHandler(this, &MainForm::mediaPlayer_PlayStateChange);
            // 
            // controlPanel
            // 
            this->controlPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(73)),
                static_cast<System::Int32>(static_cast<System::Byte>(94)));
            this->controlPanel->Controls->Add(this->positionTrackBar);
            this->controlPanel->Controls->Add(this->timeLabel);
            this->controlPanel->Controls->Add(this->playButton);
            this->controlPanel->Controls->Add(this->pauseButton);
            this->controlPanel->Controls->Add(this->previousButton);
            this->controlPanel->Controls->Add(this->nextButton);
            this->controlPanel->Controls->Add(this->resetButton);
            this->controlPanel->Controls->Add(this->shuffleButton);
            this->controlPanel->Controls->Add(this->fullScreenButton);
            this->controlPanel->Controls->Add(this->loopButton);
            this->controlPanel->Controls->Add(this->skipForwardButton);
            this->controlPanel->Controls->Add(this->skipBackwardButton);
            this->controlPanel->Controls->Add(this->volumeIcon);
            this->controlPanel->Controls->Add(this->volumeTrackBar);
            this->controlPanel->Controls->Add(this->speedIcon);
            this->controlPanel->Controls->Add(this->speedComboBox);
            this->controlPanel->Cursor = System::Windows::Forms::Cursors::Hand;
            this->controlPanel->Location = System::Drawing::Point(50, 386);
            this->controlPanel->Name = L"controlPanel";
            this->controlPanel->Size = System::Drawing::Size(637, 203);
            this->controlPanel->TabIndex = 10;
            this->controlPanel->Visible = false;
            this->controlPanel->Resize += gcnew System::EventHandler(this, &MainForm::ControlPanel_Resize);
            // 
            // positionTrackBar
            // 
            this->positionTrackBar->AutoSize = false;
            this->positionTrackBar->LargeChange = 4;
            this->positionTrackBar->Location = System::Drawing::Point(10, 10);
            this->positionTrackBar->Margin = System::Windows::Forms::Padding(2);
            this->positionTrackBar->Maximum = 100;
            this->positionTrackBar->Name = L"positionTrackBar";
            this->positionTrackBar->Size = System::Drawing::Size(621, 27);
            this->positionTrackBar->TabIndex = 0;
            this->positionTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->positionTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::PositionTrackBar_Scroll);
            this->positionTrackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseDown);
            this->positionTrackBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::PositionTrackBar_MouseUp);
            // 
            // timeLabel
            // 
            this->timeLabel->ForeColor = System::Drawing::Color::White;
            this->timeLabel->Location = System::Drawing::Point(182, 39);
            this->timeLabel->Name = L"timeLabel";
            this->timeLabel->Size = System::Drawing::Size(293, 28);
            this->timeLabel->TabIndex = 1;
            this->timeLabel->Text = L"00:00 / 00:00";
            this->timeLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            // 
            // playButton
            // 
            this->playButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->playButton->BackColor = System::Drawing::Color::Transparent;
            this->playButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"playButton.BackgroundImage")));
            this->playButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->playButton->Cursor = System::Windows::Forms::Cursors::Hand;
            this->playButton->FlatAppearance->BorderSize = 0;
            this->playButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->playButton->ForeColor = System::Drawing::Color::White;
            this->playButton->Location = System::Drawing::Point(305, 66);
            this->playButton->Name = L"playButton";
            this->playButton->Size = System::Drawing::Size(67, 73);
            this->playButton->TabIndex = 2;
            this->playButton->UseVisualStyleBackColor = false;
            this->playButton->Click += gcnew System::EventHandler(this, &MainForm::PlayButton_Click);
            // 
            // pauseButton
            // 
            this->pauseButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->pauseButton->BackColor = System::Drawing::Color::Transparent;
            this->pauseButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pauseButton.BackgroundImage")));
            this->pauseButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->pauseButton->FlatAppearance->BorderSize = 0;
            this->pauseButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->pauseButton->ForeColor = System::Drawing::Color::White;
            this->pauseButton->Location = System::Drawing::Point(308, 67);
            this->pauseButton->Name = L"pauseButton";
            this->pauseButton->Size = System::Drawing::Size(64, 70);
            this->pauseButton->TabIndex = 3;
            this->pauseButton->UseVisualStyleBackColor = false;
            this->pauseButton->Visible = false;
            this->pauseButton->Click += gcnew System::EventHandler(this, &MainForm::PauseButton_Click);
            // 
            // previousButton
            // 
            this->previousButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->previousButton->BackColor = System::Drawing::Color::Transparent;
            this->previousButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"previousButton.BackgroundImage")));
            this->previousButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->previousButton->FlatAppearance->BorderSize = 0;
            this->previousButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->previousButton->ForeColor = System::Drawing::Color::White;
            this->previousButton->Location = System::Drawing::Point(156, 66);
            this->previousButton->Name = L"previousButton";
            this->previousButton->Size = System::Drawing::Size(74, 73);
            this->previousButton->TabIndex = 4;
            this->previousButton->UseVisualStyleBackColor = false;
            this->previousButton->Click += gcnew System::EventHandler(this, &MainForm::PreviousButton_Click);
            // 
            // nextButton
            // 
            this->nextButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->nextButton->BackColor = System::Drawing::Color::Transparent;
            this->nextButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nextButton.BackgroundImage")));
            this->nextButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->nextButton->FlatAppearance->BorderSize = 0;
            this->nextButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->nextButton->ForeColor = System::Drawing::Color::White;
            this->nextButton->Location = System::Drawing::Point(448, 67);
            this->nextButton->Name = L"nextButton";
            this->nextButton->Size = System::Drawing::Size(66, 72);
            this->nextButton->TabIndex = 5;
            this->nextButton->UseVisualStyleBackColor = false;
            this->nextButton->Click += gcnew System::EventHandler(this, &MainForm::NextButton_Click);
            // 
            // resetButton
            // 
            this->resetButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->resetButton->BackColor = System::Drawing::Color::Transparent;
            this->resetButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"resetButton.BackgroundImage")));
            this->resetButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->resetButton->FlatAppearance->BorderSize = 0;
            this->resetButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->resetButton->ForeColor = System::Drawing::Color::White;
            this->resetButton->Location = System::Drawing::Point(520, 68);
            this->resetButton->Name = L"resetButton";
            this->resetButton->Size = System::Drawing::Size(70, 71);
            this->resetButton->TabIndex = 6;
            this->resetButton->UseVisualStyleBackColor = false;
            this->resetButton->Click += gcnew System::EventHandler(this, &MainForm::ResetButton_Click);
            // 
            // shuffleButton
            // 
            this->shuffleButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->shuffleButton->BackColor = System::Drawing::Color::Transparent;
            this->shuffleButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"shuffleButton.BackgroundImage")));
            this->shuffleButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->shuffleButton->FlatAppearance->BorderSize = 0;
            this->shuffleButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->shuffleButton->ForeColor = System::Drawing::Color::Transparent;
            this->shuffleButton->Location = System::Drawing::Point(78, 79);
            this->shuffleButton->Name = L"shuffleButton";
            this->shuffleButton->Size = System::Drawing::Size(97, 83);
            this->shuffleButton->TabIndex = 7;
            this->shuffleButton->UseVisualStyleBackColor = false;
            this->shuffleButton->Click += gcnew System::EventHandler(this, &MainForm::ShuffleButton_Click);
            // 
            // fullScreenButton
            // 
            this->fullScreenButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->fullScreenButton->BackColor = System::Drawing::Color::Transparent;
            this->fullScreenButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"fullScreenButton.BackgroundImage")));
            this->fullScreenButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->fullScreenButton->FlatAppearance->BorderSize = 0;
            this->fullScreenButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->fullScreenButton->ForeColor = System::Drawing::Color::White;
            this->fullScreenButton->Location = System::Drawing::Point(561, 131);
            this->fullScreenButton->Name = L"fullScreenButton";
            this->fullScreenButton->Size = System::Drawing::Size(68, 65);
            this->fullScreenButton->TabIndex = 13;
            this->fullScreenButton->UseVisualStyleBackColor = false;
            this->fullScreenButton->Click += gcnew System::EventHandler(this, &MainForm::FullScreenButton_Click);
            // 
            // loopButton
            // 
            this->loopButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->loopButton->BackColor = System::Drawing::Color::Transparent;
            this->loopButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"loopButton.BackgroundImage")));
            this->loopButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->loopButton->FlatAppearance->BorderSize = 0;
            this->loopButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->loopButton->ForeColor = System::Drawing::Color::Transparent;
            this->loopButton->Location = System::Drawing::Point(118, 141);
            this->loopButton->Name = L"loopButton";
            this->loopButton->Size = System::Drawing::Size(97, 83);
            this->loopButton->TabIndex = 14;
            this->loopButton->UseVisualStyleBackColor = false;
            this->loopButton->Click += gcnew System::EventHandler(this, &MainForm::LoopButton_Click);
            // 
            // skipForwardButton
            // 
            this->skipForwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipForwardButton->BackColor = System::Drawing::Color::Transparent;
            this->skipForwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"skipForwardButton.BackgroundImage")));
            this->skipForwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->skipForwardButton->FlatAppearance->BorderSize = 0;
            this->skipForwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipForwardButton->ForeColor = System::Drawing::Color::White;
            this->skipForwardButton->Location = System::Drawing::Point(378, 70);
            this->skipForwardButton->Name = L"skipForwardButton";
            this->skipForwardButton->Size = System::Drawing::Size(65, 65);
            this->skipForwardButton->TabIndex = 15;
            this->skipForwardButton->UseVisualStyleBackColor = false;
            this->skipForwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipForwardButton_Click);
            // 
            // skipBackwardButton
            // 
            this->skipBackwardButton->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->skipBackwardButton->BackColor = System::Drawing::Color::Transparent;
            this->skipBackwardButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"skipBackwardButton.BackgroundImage")));
            this->skipBackwardButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->skipBackwardButton->FlatAppearance->BorderSize = 0;
            this->skipBackwardButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->skipBackwardButton->ForeColor = System::Drawing::Color::White;
            this->skipBackwardButton->Location = System::Drawing::Point(229, 72);
            this->skipBackwardButton->Name = L"skipBackwardButton";
            this->skipBackwardButton->Size = System::Drawing::Size(70, 61);
            this->skipBackwardButton->TabIndex = 16;
            this->skipBackwardButton->UseVisualStyleBackColor = false;
            this->skipBackwardButton->Click += gcnew System::EventHandler(this, &MainForm::SkipBackwardButton_Click);
            // 
            // volumeIcon
            // 
            this->volumeIcon->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"volumeIcon.BackgroundImage")));
            this->volumeIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->volumeIcon->Location = System::Drawing::Point(354, 146);
            this->volumeIcon->Name = L"volumeIcon";
            this->volumeIcon->Size = System::Drawing::Size(62, 54);
            this->volumeIcon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->volumeIcon->TabIndex = 17;
            this->volumeIcon->TabStop = false;
            // 
            // volumeTrackBar
            // 
            this->volumeTrackBar->AutoSize = false;
            this->volumeTrackBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(73)),
                static_cast<System::Int32>(static_cast<System::Byte>(94)));
            this->volumeTrackBar->Location = System::Drawing::Point(413, 162);
            this->volumeTrackBar->Maximum = 100;
            this->volumeTrackBar->Name = L"volumeTrackBar";
            this->volumeTrackBar->Size = System::Drawing::Size(101, 32);
            this->volumeTrackBar->TabIndex = 9;
            this->volumeTrackBar->TickStyle = System::Windows::Forms::TickStyle::None;
            this->volumeTrackBar->Value = 50;
            this->volumeTrackBar->Scroll += gcnew System::EventHandler(this, &MainForm::VolumeTrackBar_Scroll);
            // 
            // speedIcon
            // 
            this->speedIcon->BackColor = System::Drawing::Color::Transparent;
            this->speedIcon->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"speedIcon.BackgroundImage")));
            this->speedIcon->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
            this->speedIcon->Location = System::Drawing::Point(0, 101);
            this->speedIcon->Name = L"speedIcon";
            this->speedIcon->Size = System::Drawing::Size(79, 61);
            this->speedIcon->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->speedIcon->TabIndex = 19;
            this->speedIcon->TabStop = false;
            this->speedIcon->Click += gcnew System::EventHandler(this, &MainForm::SpeedIcon_Click);
            // 
            // speedComboBox
            // 
            this->speedComboBox->AllowDrop = true;
            this->speedComboBox->BackColor = System::Drawing::Color::LightCyan;
            this->speedComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->speedComboBox->FormattingEnabled = true;
            // Items populated at runtime (constructor) to avoid designer parse issues
            this->speedComboBox->Location = System::Drawing::Point(0, 166);
            this->speedComboBox->Name = L"speedComboBox";
            this->speedComboBox->Size = System::Drawing::Size(97, 24);
            this->speedComboBox->TabIndex = 12;
            this->speedComboBox->Visible = false;
            this->speedComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::SpeedComboBox_SelectedIndexChanged);
            this->speedComboBox->DropDownClosed += gcnew System::EventHandler(this, &MainForm::SpeedComboBox_DropDownClosed);
            // 
            // playlistInfoLabel
            // 
            this->playlistInfoLabel->Location = System::Drawing::Point(0, 0);
            this->playlistInfoLabel->Name = L"playlistInfoLabel";
            this->playlistInfoLabel->Size = System::Drawing::Size(100, 23);
            this->playlistInfoLabel->TabIndex = 0;
            // 
            // timer
            // 
            this->timer->Tick += gcnew System::EventHandler(this, &MainForm::Timer_Tick);
            // 
            // listBox2
            // 
            this->listBox2->BackColor = System::Drawing::SystemColors::InactiveCaption;
            this->listBox2->FormattingEnabled = true;
            this->listBox2->ItemHeight = 16;
            this->listBox2->Location = System::Drawing::Point(137, 112);
            this->listBox2->Name = L"listBox2";
            this->listBox2->Size = System::Drawing::Size(177, 196);
            this->listBox2->TabIndex = 3;
            this->listBox2->DoubleClick += gcnew System::EventHandler(this, &MainForm::listBox2_DoubleClick);
            // 
            // pictureBoxBackground
            // 
            this->pictureBoxBackground->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxBackground->Location = System::Drawing::Point(0, 0);
            this->pictureBoxBackground->Name = L"pictureBoxBackground";
            this->pictureBoxBackground->Size = System::Drawing::Size(742, 610);
            this->pictureBoxBackground->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->pictureBoxBackground->TabIndex = 0;
            this->pictureBoxBackground->TabStop = false;
            // 
            // moreOptionsMenu
            // 
            this->moreOptionsMenu->ImageScalingSize = System::Drawing::Size(20, 20);
            this->moreOptionsMenu->Name = L"moreOptionsMenu";
            this->moreOptionsMenu->Size = System::Drawing::Size(61, 4);
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(742, 610);
            // Prevent user from resizing/strecthing the window at runtime
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximumSize = this->Size;
            this->MinimumSize = this->Size;
            this->Controls->Add(this->videoPanel);
            this->Controls->Add(this->controlPanel);
            this->Controls->Add(this->btnMoreOptions);
            this->Controls->Add(this->btnPlay);
            this->Controls->Add(this->listBox2);
            this->Controls->Add(this->btnUpload);
            this->Controls->Add(this->btnDelete);
            this->Controls->Add(this->pictureBoxBackground);
            // Ensure the background is behind other controls so it doesn't block input
            try { this->pictureBoxBackground->SendToBack(); }
            catch (Exception^) { }
            this->MaximizeBox = false;
            this->Name = L"MainForm";
            this->Text = L"Video Player";
            this->videoPanel->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mediaPlayer))->EndInit();
            this->controlPanel->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->positionTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeIcon))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->volumeTrackBar))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->speedIcon))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxBackground))->EndInit();
            this->ResumeLayout(false);

        }

        // ========== EXACT FUNCTIONS FROM SECOND CODE ==========
        // CreateMoreOptionsMenu: build context menu for additional playlist actions
        // This method constructs the context menu shown when the user clicks the
        // More Options button. It creates menu items for sorting and clearing the
        // playlist and wires their Click events to handlers in the form.
        void CreateMoreOptionsMenu()
        {
            // Create menu items
            ToolStripMenuItem^ sortAlphaItem = gcnew ToolStripMenuItem("Sort A-Z");
            sortAlphaItem->Click += gcnew System::EventHandler(this, &MainForm::sortAlpha_Click);

            ToolStripMenuItem^ sortTimeItem = gcnew ToolStripMenuItem("Sort by Time Added");
            sortTimeItem->Click += gcnew System::EventHandler(this, &MainForm::sortTime_Click);

            ToolStripSeparator^ separator = gcnew ToolStripSeparator();

            ToolStripMenuItem^ clearAllItem = gcnew ToolStripMenuItem("Clear All");
            clearAllItem->Click += gcnew System::EventHandler(this, &MainForm::clearAll_Click);

            // Add items to menu
            moreOptionsMenu->Items->Add(sortAlphaItem);
            moreOptionsMenu->Items->Add(sortTimeItem);
            moreOptionsMenu->Items->Add(separator);
            moreOptionsMenu->Items->Add(clearAllItem);
        }

        // PositionListBox: compute and set list box location and size responsively
        // Arranges the visible list box that shows videos/playlists. It computes
        // positions and sizes based on the form's client area so the UI scales
        // reasonably when the form size changes.
        void PositionListBox()
        {
            double topLinePercent = 24.5;
            double bottomLinePercent = 75.0;
            double leftPercent = 33.5;
            double rightMarginPercent = 37.0;

            int topPosition = (int)(this->ClientSize.Height * topLinePercent / 100.0);
            int bottomPosition = (int)(this->ClientSize.Height * bottomLinePercent / 100.0);
            int leftPosition = (int)(this->ClientSize.Width * leftPercent / 100.0);
            int rightMargin = (int)(this->ClientSize.Width * rightMarginPercent / 100.0);

            listBox2->Location = System::Drawing::Point(leftPosition, topPosition);
            listBox2->Size = System::Drawing::Size(
                this->ClientSize.Width - leftPosition - rightMargin,
                bottomPosition - topPosition
            );
        }

        // PositionButtons: compute and position action buttons based on client size
        // Lays out top-level action buttons (delete/play/upload) and the More
        // Options button using percentages of the client area so placement
        // remains consistent across resolutions.
        void PositionButtons()
        {
            double buttonWidthPercent = 9.0;
            double buttonHeightPercent = 12.0;

            int buttonWidth = (int)(this->ClientSize.Width * buttonWidthPercent / 100.0);
            int buttonHeight = (int)(this->ClientSize.Height * buttonHeightPercent / 100.0);

            double buttonRowTopPercent = 78.0;
            int buttonTop = (int)(this->ClientSize.Height * buttonRowTopPercent / 100.0);

            double spacingPercent = 2.0;
            int spacing = (int)(this->ClientSize.Width * spacingPercent / 100.0);

            double startLeftPercent = 32.9;
            int startLeft = (int)(this->ClientSize.Width * startLeftPercent / 100.0);

            btnDelete->Location = System::Drawing::Point(startLeft, buttonTop);
            btnDelete->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            int playLeft = startLeft + buttonWidth + spacing;
            btnPlay->Location = System::Drawing::Point(playLeft, buttonTop);
            btnPlay->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            int uploadLeft = playLeft + buttonWidth + spacing;
            btnUpload->Location = System::Drawing::Point(uploadLeft, buttonTop);
            btnUpload->Size = System::Drawing::Size(buttonWidth, buttonHeight);

            // Position the More Options button (3 dots)
            double newButtonWidthPercent = 13.0;
            double newButtonHeightPercent = 9.0;
            int newButtonWidth = (int)(this->ClientSize.Width * newButtonWidthPercent / 100.0);
            int newButtonHeight = (int)(this->ClientSize.Height * newButtonHeightPercent / 100.0);

            double newButtonLeftPercent = 67.0;
            int newButtonLeft = (int)(this->ClientSize.Width * newButtonLeftPercent / 100.0);

            double newButtonTopPercent = 24.5;
            int newButtonTop = (int)(this->ClientSize.Height * newButtonTopPercent / 100.0);

            btnMoreOptions->Location = System::Drawing::Point(newButtonLeft, newButtonTop);
            btnMoreOptions->Size = System::Drawing::Size(newButtonWidth, newButtonHeight);
        }

        // PositionMediaPlayer: layout video panel and media player control
        // Positions the video display panel and the embedded Windows Media
        // Player control within it. This method also triggers aspect-ratio
        // adjustments so videos cover the panel neatly without stretching.
        void PositionMediaPlayer()
        {
            // Place the video panel more toward the top of the window
            int panelWidth = (int)(this->ClientSize.Width * 0.85);
            // reduce height so panel sits higher and leaves room for controls below
            int panelHeight = (int)(this->ClientSize.Height * 0.60);
            int panelLeft = (this->ClientSize.Width - panelWidth) / 2;
            // small top margin (percent of client height)
            double topPercent = 4.0;
            int panelTop = (int)(this->ClientSize.Height * topPercent / 100.0);

            videoPanel->Location = System::Drawing::Point(panelLeft, panelTop);
            videoPanel->Size = System::Drawing::Size(panelWidth, panelHeight);

            btnBackToMenu->Location = System::Drawing::Point(10, 10);

            // exit fullscreen button removed

            // Make media player cover available panel area initially, then adjust to aspect ratio and center
            // use designer offset so control aligns as expected
            mediaPlayer->Location = System::Drawing::Point(-3, -41);
            mediaPlayer->Size = System::Drawing::Size(panelWidth, panelHeight);
            this->AdjustMediaPlayerAspect();
        }

        // AdjustMediaPlayerAspect: scale/crop media to cover the video panel while preserving aspect
        // Reads media metadata (when available) and sets the media player's
        // size and location so the video covers the panel. Falls back to
        // filling the panel when no metadata is present.
        void AdjustMediaPlayerAspect()
        {
            try
            {
                if (mediaPlayer == nullptr || videoPanel == nullptr) return;

                // Only adjust when not docked fill (fullscreen)
                if (mediaPlayer->Dock == System::Windows::Forms::DockStyle::Fill) return;

                int panelWidth = videoPanel->ClientSize.Width;
                int panelHeight = videoPanel->ClientSize.Height;

                double vidW = 0.0;
                double vidH = 0.0;

                if (mediaPlayer->currentMedia != nullptr)
                {
                    try { vidW = (double)mediaPlayer->currentMedia->imageSourceWidth; } catch (Exception^) { vidW = 0.0; }
                    try { vidH = (double)mediaPlayer->currentMedia->imageSourceHeight; } catch (Exception^) { vidH = 0.0; }
                    if (vidW <= 0 || vidH <= 0)
                    {
                        try
                        {
                            String^ w = mediaPlayer->currentMedia->getItemInfo("WM/VideoWidth");
                            String^ h = mediaPlayer->currentMedia->getItemInfo("WM/VideoHeight");
                            if (!String::IsNullOrEmpty(w)) vidW = Double::Parse(w);
                            if (!String::IsNullOrEmpty(h)) vidH = Double::Parse(h);
                        }
                        catch (Exception^) { }
                    }
                }

                if (vidW > 0 && vidH > 0)
                {
                    // Use cover scaling so the video fills the panel (may crop edges) and removes top/bottom letterbox gaps
                    double scale = Math::Max((double)panelWidth / vidW, (double)panelHeight / vidH);
                    int targetW = (int)Math::Max(1.0, vidW * scale);
                    int targetH = (int)Math::Max(1.0, vidH * scale);
                    int left = (panelWidth - targetW) / 2;
                    int top = (panelHeight - targetH) / 2;
                    mediaPlayer->Location = System::Drawing::Point(left, top);
                    mediaPlayer->Size = System::Drawing::Size(targetW, targetH);
                    mediaPlayer->stretchToFit = false; // preserve aspect and center vertically/horizontally
                }
                else
                {
                    // No video info: fill the panel
                    mediaPlayer->Location = System::Drawing::Point(0, 0);
                    mediaPlayer->Size = System::Drawing::Size(panelWidth, panelHeight);
                    mediaPlayer->stretchToFit = true;
                }

                // Ensure back button stays on top
                if (btnBackToMenu != nullptr) btnBackToMenu->BringToFront();
            }
            catch (Exception^ ex)
            {
                System::Diagnostics::Debug::WriteLine("AdjustMediaPlayerAspect failed: " + ex->Message);
            }
        }

        // LoadButtonImages: attempt to load custom images for UI buttons from Images folder
        // Loads button background images from an "Images" folder adjacent to the
        // application. If images are missing the method applies sensible defaults
        // so the UI remains usable.
        void LoadButtonImages()
        {
            try
            {
                String^ basePath = Application::StartupPath;

                String^ deleteImagePath = Path::Combine(basePath, "Images\\btndelete.jpg");
                if (File::Exists(deleteImagePath))
                {
                    btnDelete->BackgroundImage = Image::FromFile(deleteImagePath);
                    btnDelete->BackgroundImageLayout = ImageLayout::Stretch;
                    btnDelete->Text = "";
                    btnDelete->FlatStyle = FlatStyle::Flat;
                    btnDelete->FlatAppearance->BorderSize = 0;
                    btnDelete->BackColor = Color::Transparent;
                }

                String^ uploadImagePath = Path::Combine(basePath, "Images\\btnupload.jpg");
                if (File::Exists(uploadImagePath))
                {
                    btnUpload->BackgroundImage = Image::FromFile(uploadImagePath);
                    btnUpload->BackgroundImageLayout = ImageLayout::Stretch;
                    btnUpload->Text = "";
                    btnUpload->FlatStyle = FlatStyle::Flat;
                    btnUpload->FlatAppearance->BorderSize = 0;
                    btnUpload->BackColor = Color::Transparent;
                }

                String^ playImagePath = Path::Combine(basePath, "Images\\btnplay.jpg");
                if (File::Exists(playImagePath))
                {
                    btnPlay->BackgroundImage = Image::FromFile(playImagePath);
                    btnPlay->BackgroundImageLayout = ImageLayout::Stretch;
                    btnPlay->Text = "";
                    btnPlay->FlatStyle = FlatStyle::Flat;
                    btnPlay->FlatAppearance->BorderSize = 0;
                    btnPlay->BackColor = Color::Transparent;
                }

                // Back button image (load from Images\btnback.png into BackgroundImage)
                String^ backImagePath = Path::Combine(basePath, "Images\\backbtn.png");
                if (btnBackToMenu != nullptr) {
                    try {
                        if (File::Exists(backImagePath)) {
                            btnBackToMenu->BackgroundImage = Image::FromFile(backImagePath);
                            btnBackToMenu->BackgroundImageLayout = ImageLayout::Stretch;
                            btnBackToMenu->Text = L"";
                            btnBackToMenu->FlatAppearance->BorderSize = 0;
                            btnBackToMenu->BackColor = Color::Transparent;
                        }
                        else {
                            // fallback text if image missing
                            btnBackToMenu->Text = L"\u2190 Back"; // left arrow + Back
                            btnBackToMenu->ForeColor = Color::White;
                            btnBackToMenu->FlatStyle = FlatStyle::Flat;
                            btnBackToMenu->FlatAppearance->BorderSize = 0;
                        }
                    }
                    catch (Exception^) {
                        // ignore image load failures, fall back to text
                        btnBackToMenu->Text = L"\u2190 Back";
                    }
                }

                // Load More Options button image
                String^ filterImagePath = Path::Combine(basePath, "Images\\filterbtn.jpeg");
                if (File::Exists(filterImagePath))
                {
                    btnMoreOptions->BackgroundImage = Image::FromFile(filterImagePath);
                    btnMoreOptions->BackgroundImageLayout = ImageLayout::Stretch;
                    btnMoreOptions->Text = "";
                    btnMoreOptions->FlatStyle = FlatStyle::Flat;
                    btnMoreOptions->FlatAppearance->BorderSize = 0;
                    btnMoreOptions->BackColor = Color::Transparent;
                }
                else
                {
                    // Fallback styling if image not found
                    btnMoreOptions->FlatStyle = FlatStyle::Flat;
                    btnMoreOptions->FlatAppearance->BorderSize = 1;
                    btnMoreOptions->BackColor = Color::FromArgb(240, 240, 240);
                    btnMoreOptions->ForeColor = Color::Black;
                }

                // ========== ADDITIONAL IMAGES FROM FIRST CODE ==========
                String^ fullScreenImagePath = Path::Combine(basePath, "Images\\btnfullscreen.png");
                if (File::Exists(fullScreenImagePath))
                {
                    fullScreenButton->BackgroundImage = Image::FromFile(fullScreenImagePath);
                    fullScreenButton->BackgroundImageLayout = ImageLayout::Stretch;
                    fullScreenButton->Text = "";
                    fullScreenButton->FlatStyle = FlatStyle::Flat;
                    fullScreenButton->FlatAppearance->BorderSize = 0;
                    fullScreenButton->BackColor = Color::Transparent;
                    fullScreenButton->Size = System::Drawing::Size(60, 55);
                }

                // exit fullscreen button removed

                String^ playBtnImg = Path::Combine(basePath, "Images\\playbtn.png");
                if (playButton != nullptr) {
                    if (File::Exists(playBtnImg)) {
                        playButton->BackgroundImage = Image::FromFile(playBtnImg);
                        playButton->BackgroundImageLayout = ImageLayout::Stretch;
                        playButton->Text = "";
                        playButton->FlatStyle = FlatStyle::Flat;
                        playButton->FlatAppearance->BorderSize = 0;
                        playButton->BackColor = Color::Transparent;
                    }
                    else {
                        // Fallback: show a Unicode play glyph so the button is visible even without an image
                        playButton->BackgroundImage = nullptr;
                        playButton->Text = L"\u25B6"; // play triangle
                        playButton->Font = (gcnew System::Drawing::Font(L"Segoe UI", 24, System::Drawing::FontStyle::Bold));
                        playButton->ForeColor = System::Drawing::Color::White;
                        playButton->FlatStyle = FlatStyle::Flat;
                        playButton->FlatAppearance->BorderSize = 0;
                        playButton->BackColor = System::Drawing::Color::Transparent;
                    }
                }

                String^ pauseBtnImg = Path::Combine(basePath, "Images\\btnpause.png");
                if (pauseButton != nullptr && File::Exists(pauseBtnImg)) {
                    pauseButton->BackgroundImage = Image::FromFile(pauseBtnImg);
                    pauseButton->BackgroundImageLayout = ImageLayout::Stretch;
                    pauseButton->Text = "";
                    pauseButton->FlatStyle = FlatStyle::Flat;
                    pauseButton->FlatAppearance->BorderSize = 0;
                    pauseButton->BackColor = Color::Transparent;
                }

                String^ prevImg = Path::Combine(basePath, "Images\\btnprev.png");
                if (previousButton != nullptr && File::Exists(prevImg)) {
                    previousButton->BackgroundImage = Image::FromFile(prevImg);
                    previousButton->BackgroundImageLayout = ImageLayout::Stretch;
                    previousButton->Text = "";
                    previousButton->FlatStyle = FlatStyle::Flat;
                    previousButton->FlatAppearance->BorderSize = 0;
                    previousButton->BackColor = Color::Transparent;
                }

                String^ nextImg = Path::Combine(basePath, "Images\\btnnext.png");
                if (nextButton != nullptr && File::Exists(nextImg)) {
                    nextButton->BackgroundImage = Image::FromFile(nextImg);
                    nextButton->BackgroundImageLayout = ImageLayout::Stretch;
                    nextButton->Text = "";
                    nextButton->FlatStyle = FlatStyle::Flat;
                    nextButton->FlatAppearance->BorderSize = 0;
                    nextButton->BackColor = Color::Transparent;
                }

                String^ resetImg = Path::Combine(basePath, "Images\\restbtn.png");
                if (resetButton != nullptr && File::Exists(resetImg)) {
                    resetButton->BackgroundImage = Image::FromFile(resetImg);
                    resetButton->BackgroundImageLayout = ImageLayout::Stretch;
                    resetButton->Text = "";
                    resetButton->FlatStyle = FlatStyle::Flat;
                    resetButton->FlatAppearance->BorderSize = 0;
                    resetButton->BackColor = Color::Transparent;
                    resetButton->BringToFront();
                    resetButton->Size = System::Drawing::Size(70, 71);
                }

                String^ shuffleImg = Path::Combine(basePath, "Images\\btnshuffle.png");
                if (shuffleButton != nullptr && File::Exists(shuffleImg)) {
                    shuffleButton->BackgroundImage = Image::FromFile(shuffleImg);
                    shuffleButton->BackgroundImageLayout = ImageLayout::Stretch;
                    shuffleButton->Text = "";
                    shuffleButton->FlatStyle = FlatStyle::Flat;
                    shuffleButton->FlatAppearance->BorderSize = 0;
                    shuffleButton->BackColor = Color::Transparent;
                    shuffleButton->Size = System::Drawing::Size(72, 48);
                    shuffleButton->AccessibilityObject->Name = "Shuffle Button";
                }

                String^ loopImg = Path::Combine(basePath, "Images\\btnloop.png");
                // loop button image will be set by UpdateLoopButtonImage() to reflect on/off state

                String^ forwardImg = Path::Combine(basePath, "Images\\btnskipforward.png");
                if (skipForwardButton != nullptr && File::Exists(forwardImg)) {
                    skipForwardButton->BackgroundImage = Image::FromFile(forwardImg);
                    skipForwardButton->BackgroundImageLayout = ImageLayout::Stretch;
                    skipForwardButton->Text = "";
                    skipForwardButton->FlatStyle = FlatStyle::Flat;
                    skipForwardButton->FlatAppearance->BorderSize = 0;
                    skipForwardButton->BackColor = Color::Transparent;
                }

                String^ backwardImg = Path::Combine(basePath, "Images\\btnskipbackward.png");
                if (skipBackwardButton != nullptr && File::Exists(backwardImg)) {
                    skipBackwardButton->BackgroundImage = Image::FromFile(backwardImg);
                    skipBackwardButton->BackgroundImageLayout = ImageLayout::Stretch;
                    skipBackwardButton->Text = "";
                    skipBackwardButton->FlatStyle = FlatStyle::Flat;
                    skipBackwardButton->FlatAppearance->BorderSize = 0;
                    skipBackwardButton->BackColor = Color::Transparent;
                }

                String^ volumeIconPath = Path::Combine(basePath, "Images\\btnvolume.png");
                if (volumeIcon != nullptr && File::Exists(volumeIconPath)) {
                    volumeIcon->Image = Image::FromFile(volumeIconPath);
                    volumeIcon->SizeMode = PictureBoxSizeMode::StretchImage;
                    volumeIcon->Visible = true;
                }

                String^ speedIconPath = Path::Combine(basePath, "Images\\speedicon.png");
                if (speedIcon != nullptr && File::Exists(speedIconPath)) {
                    speedIcon->Image = Image::FromFile(speedIconPath);
                    speedIcon->SizeMode = PictureBoxSizeMode::StretchImage;
                    speedIcon->Visible = true;
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading button images: " + ex->Message);
            }
        }

        // UpdateLoopButtonImage: set loop button image according to loopEnabled state
        // Updates the visual appearance of the loop toggle button based on the
        // current logical state (on/off). Looks for state-specific images and
        // falls back to a generic icon when unavailable.
        void UpdateLoopButtonImage()
        {
            try {
                String^ basePath = Application::StartupPath;
                String^ onImg = Path::Combine(basePath, "Images\\btnloop_on.png");
                String^ offImg = Path::Combine(basePath, "Images\\btnloop_off.png");
                String^ genericImg = Path::Combine(basePath, "Images\\btnloop.png");
                // prefer state-specific images, else fallback to a generic loop image if available
                if (loopButton == nullptr) return;
                if (loopEnabled)
                {
                    if (File::Exists(onImg)) {
                        loopButton->BackgroundImage = Image::FromFile(onImg);
                        loopButton->BackgroundImageLayout = ImageLayout::Stretch;
                    }
                    else if (File::Exists(genericImg)) {
                        loopButton->BackgroundImage = Image::FromFile(genericImg);
                        loopButton->BackgroundImageLayout = ImageLayout::Stretch;
                    }
                }
                else
                {
                    if (File::Exists(offImg)) {
                        loopButton->BackgroundImage = Image::FromFile(offImg);
                        loopButton->BackgroundImageLayout = ImageLayout::Stretch;
                    }
                    else if (File::Exists(genericImg)) {
                        // no explicit 'off' image available, use generic loop icon
                        loopButton->BackgroundImage = Image::FromFile(genericImg);
                        loopButton->BackgroundImageLayout = ImageLayout::Stretch;
                    }
                    else {
                        // nothing found: ensure button still shows a visible state (optional)
                        loopButton->BackgroundImage = nullptr;
                    }
                }
            } catch (Exception^) { }
        }

        // LoadBackgroundImage: load background image or fallback to solid color
        // Attempts to load a full-window background image. If not found, a
        // message is shown and the form retains its default background color.
        void LoadBackgroundImage()
        {
            try
            {
                String^ imagePath = Path::Combine(Application::StartupPath, "Images\\background.jpg");

                if (File::Exists(imagePath))
                {
                    pictureBoxBackground->Image = Image::FromFile(imagePath);
                    pictureBoxBackground->SendToBack();
                }
                else
                {
                    MessageBox::Show("Image not found at: " + imagePath);
                }
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error loading image: " + ex->Message);
            }
        }

        // ShowVideoPanel: display the video panel and hide menu controls
        // Makes the video playback area visible and hides the playlist UI so
        // the user focuses on the media. Also brings the control panel to the
        // front so playback controls are accessible.
        void ShowVideoPanel()
        {
            videoPanel->Visible = true;
            videoPanel->BringToFront();
            controlPanel->Visible = true;  // ADD THIS LINE
            controlPanel->BringToFront();  // ADD THIS LINE

            listBox2->Visible = false;
            btnDelete->Visible = false;
            btnUpload->Visible = false;
            btnPlay->Visible = false;
            btnMoreOptions->Visible = false;
        }

        // ShowMenu: return to the main menu view and hide the video panel
        // Reverses ShowVideoPanel, restoring playlist controls and hiding the
        // playback UI. This is used when exiting playback mode.
        void ShowMenu()
        {
            videoPanel->Visible = false;
            controlPanel->Visible = false;  // ADD THIS LINE

            listBox2->Visible = true;
            btnDelete->Visible = true;
            btnUpload->Visible = true;
            btnPlay->Visible = true;
            btnMoreOptions->Visible = true;
        }

        // btnBackToMenu_Click: handler to stop playback and show menu
        // Click handler for the back button inside the video panel. Stops
        // playback if active and returns to the playlist/menu UI.
        System::Void btnBackToMenu_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (isPlaying)
            {
                mediaPlayer->Ctlcontrols->stop();
                isPlaying = false;
            }

            ShowMenu();
        }

        // btnMoreOptions_Click: show the context menu anchored to the MoreOptions button
        // Opens the More Options context menu positioned below the button.
        System::Void btnMoreOptions_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Show the context menu below the button
            Point buttonLocation = btnMoreOptions->PointToScreen(Point(0, btnMoreOptions->Height));
            moreOptionsMenu->Show(buttonLocation);
        }

        // sortAlpha_Click: sort playlist alphabetically and refresh list
        // Handler to trigger alphabetic sorting of the playlist display.
        System::Void sortAlpha_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortAlphabetically(listBox2);
        }

        // sortTime_Click: sort playlist by time added and refresh list
        // Handler to sort playlist items by their addition time (oldest first).
        System::Void sortTime_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->sortByTimeAdded(listBox2);
        }

        // clearAll_Click: clear all playlists and videos after confirmation
        // Handler to remove all items from the playlist after user
        // confirmation. Calls VideoList::clearAll to perform the operation.
        System::Void clearAll_Click(System::Object^ sender, System::EventArgs^ e)
        {
            videoList->clearAll(listBox2);
        }

        // btnPlay_Click: start playback of the selected video or expand playlist header
        // Starts playback of the currently selected video item. If the
        // selected item is a playlist header the playlist expansion is toggled.
        System::Void btnPlay_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                VideoList::Node^ selectedNode = videoList->getNodeAtDisplayIndex(listBox2->SelectedIndex, listBox2);

                if (selectedNode != nullptr)
                {
                    if (selectedNode->isPlaylistHeader)
                    {
                        videoList->togglePlaylistExpansion(listBox2->SelectedIndex, listBox2);
                        return;
                    }

                    videoList->setCurrentNode(selectedNode);
                }
            }

            String^ videoPath = videoList->getCurrentNodePath();

            if (videoPath != nullptr && File::Exists(videoPath))
            {
                mediaPlayer->URL = videoPath;
                ShowVideoPanel();
                mediaPlayer->Ctlcontrols->play();
                ApplySelectedPlaybackRate();
                isPlaying = true;
                controlPanel->Visible = true;
            }
            else
            {
                MessageBox::Show("Please select a video to play.", "No Video Selected",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        // listBox2_DoubleClick: wrapper to trigger play on double-clicked item
        // Convenience wrapper so double-clicking an item invokes the same
        // logic as clicking the Play button.
        System::Void listBox2_DoubleClick(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex >= 0)
            {
                btnPlay_Click(sender, e);
            }
        }

        // mediaPlayer_PlayStateChange: handle media player state changes (ended, playing, stopped)
        // Responds to Windows Media Player state transitions. Handles end-of-media
        // behavior (advance or loop), updates UI playback flags, and manages
        // the timer used to refresh the position slider.
        System::Void mediaPlayer_PlayStateChange(System::Object^ sender, AxWMPLib::_WMPOCXEvents_PlayStateChangeEvent^ e)
        {
            if (e->newState == 8) // MediaEnded
            {
                // If loop enabled -> restart current video (single-item loop)
                if (loopEnabled)
                {
                    try {
                        // restart from beginning
                        mediaPlayer->Ctlcontrols->currentPosition = 0;
                        mediaPlayer->Ctlcontrols->play();
                        ApplySelectedPlaybackRate();
                        isPlaying = true;
                        controlPanel->Visible = true;
                        return; // keep playing
                    } catch (Exception^) {
                        // fall through to normal behavior
                    }
                }

                // not looping current: try to play next in list
                String^ nextPath = videoList->nextVideo();
                if (nextPath != nullptr)
                {
                    mediaPlayer->URL = nextPath;
                    mediaPlayer->Ctlcontrols->play();
                    ApplySelectedPlaybackRate();
                    isPlaying = true;
                    controlPanel->Visible = true;
                }
                else
                {
                    // Loop playlist if enabled (legacy behaviour)
                    if (!loopEnabled && listBox2->Items->Count > 0)
                    {
                        listBox2->SelectedIndex = 0;
                        videoList->setCurrentNode(0);
                        String^ path = videoList->getCurrentNodePath();
                        if (path != nullptr)
                        {
                            mediaPlayer->URL = path;
                            mediaPlayer->Ctlcontrols->play();
                            ApplySelectedPlaybackRate();
                            isPlaying = true;
                            controlPanel->Visible = true;
                        }
                    }
                    else
                    {
                        isPlaying = false;
                    }
                }
            }
            else if (e->newState == 1) // Stopped
            {
                isPlaying = false;
                timer->Enabled = false;  // ADD THIS
            }
            else if (e->newState == 3) // Playing
            {
                isPlaying = true;
                controlPanel->Visible = true;  // ADD THIS
                timer->Interval = 200;  // ADD THIS
                timer->Enabled = true;  // ADD THIS
                this->AdjustMediaPlayerAspect();
            }
        }

        // btnUpload_Click: show dialog to add single videos or playlists to the list
        // Presents a small dialog allowing the user to choose between adding
        // single videos or creating a new playlist. Files chosen are added to
        // the VideoList and the display is refreshed.
        System::Void btnUpload_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Create custom dialog
            Form^ uploadDialog = gcnew Form();
            uploadDialog->Text = "Upload Content";
            uploadDialog->Width = 400;
            uploadDialog->Height = 200;
            uploadDialog->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            uploadDialog->StartPosition = FormStartPosition::CenterParent;
            uploadDialog->MaximizeBox = false;
            uploadDialog->MinimizeBox = false;

            Label^ label = gcnew Label();
            label->Text = "What would you like to upload?";
            label->Location = System::Drawing::Point(20, 20);
            label->AutoSize = true;
            label->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));

            Button^ btnVideo = gcnew Button();
            btnVideo->Text = "Single Video";
            btnVideo->Location = System::Drawing::Point(50, 70);
            btnVideo->Size = System::Drawing::Size(140, 50);
            btnVideo->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            btnVideo->DialogResult = System::Windows::Forms::DialogResult::Yes;

            Button^ btnPlaylist = gcnew Button();
            btnPlaylist->Text = "Playlist";
            btnPlaylist->Location = System::Drawing::Point(210, 70);
            btnPlaylist->Size = System::Drawing::Size(140, 50);
            btnPlaylist->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            btnPlaylist->DialogResult = System::Windows::Forms::DialogResult::No;

            uploadDialog->Controls->Add(label);
            uploadDialog->Controls->Add(btnVideo);
            uploadDialog->Controls->Add(btnPlaylist);
            uploadDialog->AcceptButton = btnVideo;
            uploadDialog->CancelButton = btnPlaylist;

            System::Windows::Forms::DialogResult result = uploadDialog->ShowDialog();

            if (result == System::Windows::Forms::DialogResult::Yes)
            {
                // Upload individual videos
                OpenFileDialog^ ofd = gcnew OpenFileDialog();
                ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov;*.wmv;*.flv";
                ofd->Multiselect = true;

                if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                {
                    for each (String ^ fullPath in ofd->FileNames)
                    {
                        String^ name = Path::GetFileName(fullPath);
                        videoList->addVideo(fullPath, name, listBox2);
                    }
                }
            }



            else if (result == System::Windows::Forms::DialogResult::No)
            {
                // Upload playlist
                String^ playlistName = nullptr;

                Form^ inputForm = gcnew Form();
                inputForm->Text = "New Playlist";
                inputForm->Width = 350;
                inputForm->Height = 150;
                inputForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                inputForm->StartPosition = FormStartPosition::CenterParent;
                inputForm->MaximizeBox = false;
                inputForm->MinimizeBox = false;

                Label^ label2 = gcnew Label();
                label2->Text = "Enter playlist name:";
                label2->Location = System::Drawing::Point(10, 20);
                label2->AutoSize = true;

                TextBox^ textBox = gcnew TextBox();
                textBox->Location = System::Drawing::Point(10, 45);
                textBox->Width = 310;

                Button^ btnOK = gcnew Button();
                btnOK->Text = "OK";
                btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
                btnOK->Location = System::Drawing::Point(150, 75);

                Button^ btnCancel = gcnew Button();
                btnCancel->Text = "Cancel";
                btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
                btnCancel->Location = System::Drawing::Point(235, 75);

                inputForm->Controls->Add(label2);
                inputForm->Controls->Add(textBox);
                inputForm->Controls->Add(btnOK);
                inputForm->Controls->Add(btnCancel);
                inputForm->AcceptButton = btnOK;
                inputForm->CancelButton = btnCancel;

                if (inputForm->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                {
                    playlistName = textBox->Text->Trim();

                    if (String::IsNullOrWhiteSpace(playlistName))
                    {
                        MessageBox::Show("Playlist name cannot be empty!", "Error",
                            MessageBoxButtons::OK, MessageBoxIcon::Error);
                        return;
                    }

                    OpenFileDialog^ ofd = gcnew OpenFileDialog();
                    ofd->Filter = "Video Files|*.mp4;*.avi;*.mkv;*.mov;*.wmv;*.flv";
                    ofd->Multiselect = true;
                    ofd->Title = "Select videos for playlist: " + playlistName;

                        if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                        {
                            if (ofd->FileNames->Length > 0)
                            {
                                // convert array to pass to addPlaylist (designer-safe)
                                videoList->addPlaylist(ofd->FileNames, playlistName, listBox2);
                            }
                        }
                }
            }
        }

        // btnDelete_Click: delete selected item from playlist (with validation)
        // Deletes the currently selected display item (video or playlist with
        // its children) after confirming a valid selection.
        System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (listBox2->SelectedIndex == -1)
            {
                MessageBox::Show("Please select a video to delete.", "Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            videoList->removeItem(listBox2->SelectedIndex, listBox2);
        }

        // AutoSavePlaylist: persist current playlist to an autosave file
        // Saves the current playlist state to a file in the application
        // directory so the playlist can be restored on next launch.
        void AutoSavePlaylist()
        {
            try
            {
                String^ autoSavePath = Path::Combine(Application::StartupPath, "playlist_autosave.txt");
                videoList->saveToFile(autoSavePath, listBox2);
            }
            catch (Exception^ ex)
            {
                System::Diagnostics::Debug::WriteLine("Auto-save failed: " + ex->Message);
            }
        }

        // AutoLoadPlaylist: load playlist from autosave file if present
        // Loads the autosaved playlist file at startup if present and populates
        // the UI list box accordingly.
        void AutoLoadPlaylist()
        {
            try
            {
                String^ autoSavePath = Path::Combine(Application::StartupPath, "playlist_autosave.txt");

                if (File::Exists(autoSavePath))
                {
                    videoList->loadFromFile(autoSavePath, listBox2);
                }
            }
            catch (Exception^ ex)
            {
                System::Diagnostics::Debug::WriteLine("Auto-load failed: " + ex->Message);
            }
        }

        // ========== FUNCTIONS FROM FIRST CODE THAT ARE NOT IN SECOND CODE ==========
        // ArrangeControlsCentered: layout control panel buttons centered within panel
        // Arranges playback control buttons in the center of the control
        // panel. This method is used when entering fullscreen to produce a
        // centered control layout.
        void ArrangeControlsCentered()
        {
            // Replace prior centered algorithm with the app-provided ArrangeButtons behavior
            if (controlPanel == nullptr) return;
            // progressBar at top
            try {
                int panelW = controlPanel->ClientSize.Width;
                int panelH = controlPanel->ClientSize.Height;
                if (positionTrackBar != nullptr) {
                    positionTrackBar->Location = System::Drawing::Point(10, 6);
                    positionTrackBar->Width = Math::Max(200, panelW - 20);
                }

                if (timeLabel != nullptr) {
                    timeLabel->AutoSize = true;
                    timeLabel->Location = System::Drawing::Point(panelW / 2 - (timeLabel->Width/2), positionTrackBar->Bottom + 4);
                }

                // Buttons layout similar to provided sample
                int buttonY = positionTrackBar->Bottom + 18;
                int normalW = 50;
                int normalH = 50;
                int largeW = 60;
                int largeH = 60;
                int volumeSliderW = 100;
                int spacing = 15;

                // compute total width
                int totalButtons = 9; // volume button + slider + 7 other buttons
                int totalWidth = 0;
                totalWidth += (normalW * 7) + largeW + volumeSliderW;
                totalWidth += spacing * (totalButtons - 1);
                int startX = (panelW - totalWidth) / 2;
                // ensure we don't start off the left edge when controls wider than panel
                if (startX < 10) startX = 10;

                int x = startX;
                if (volumeIcon != nullptr) { volumeIcon->Size = System::Drawing::Size(normalW, normalH); volumeIcon->Location = System::Drawing::Point(x, buttonY); x += normalW + 5; }
                if (volumeTrackBar != nullptr) { volumeTrackBar->Size = System::Drawing::Size(volumeSliderW, 28); volumeTrackBar->Location = System::Drawing::Point(x, buttonY + 10); x += volumeSliderW + spacing; }

                // place shuffle button left of the main controls
                if (shuffleButton != nullptr) { shuffleButton->Size = System::Drawing::Size(normalW, normalH); shuffleButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }

                if (previousButton != nullptr) { previousButton->Size = System::Drawing::Size(normalW, normalH); previousButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }
                if (skipBackwardButton != nullptr) { skipBackwardButton->Size = System::Drawing::Size(normalW, normalH); skipBackwardButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }
                if (playButton != nullptr) { playButton->Size = System::Drawing::Size(largeW, largeH); playButton->Location = System::Drawing::Point(x, buttonY - 5); x += largeW + spacing; }
                if (skipForwardButton != nullptr) { skipForwardButton->Size = System::Drawing::Size(normalW, normalH); skipForwardButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }
                if (nextButton != nullptr) { nextButton->Size = System::Drawing::Size(normalW, normalH); nextButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }
                if (resetButton != nullptr) { resetButton->Size = System::Drawing::Size(normalW, normalH); resetButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }
                if (fullScreenButton != nullptr) { fullScreenButton->Size = System::Drawing::Size(normalW, normalH); fullScreenButton->Location = System::Drawing::Point(x, buttonY); x += normalW + spacing; }

                // make sure fullscreen button didn't end up outside the control panel
                try {
                    if (fullScreenButton != nullptr) {
                        int rightBound = fullScreenButton->Right;
                        if (rightBound > panelW - 10) {
                            int newLeft = System::Math::Max(10, panelW - 10 - fullScreenButton->Width);
                            fullScreenButton->Left = newLeft;
                        }
                    }
                } catch (Exception^) { }

                // ensure pause overlays play
                if (playButton != nullptr && pauseButton != nullptr) { pauseButton->Location = playButton->Location; pauseButton->Size = playButton->Size; pauseButton->BringToFront(); }
            }
            catch (Exception^) { }
        }

        // PlayButton_Click: resume playback and update play/pause button visibility
        // Resumes media playback and updates the visible play/pause controls.
        System::Void PlayButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->URL != nullptr && mediaPlayer->URL != "") {
                mediaPlayer->Ctlcontrols->play();
                ApplySelectedPlaybackRate();
                playButton->Visible = false;
                pauseButton->Visible = true;
                isPlaying = true;
                if (isFullscreen) ArrangeControlsCentered();
            }
        }

        // PauseButton_Click: pause playback and toggle controls
        // Pauses playback and toggles the visible play/pause buttons.
        System::Void PauseButton_Click(System::Object^ sender, System::EventArgs^ e) {
            mediaPlayer->Ctlcontrols->pause();
            pauseButton->Visible = false;
            playButton->Visible = true;
            isPlaying = false;
            if (isFullscreen) ArrangeControlsCentered();
        }

        // NextButton_Click: advance to next video in the playlist
        // Advances playback to the next available video in the current
        // playlist (or global list) and starts playback.
        System::Void NextButton_Click(System::Object^ sender, System::EventArgs^ e) {
            String^ nextPath = videoList->nextVideo();
            if (nextPath != nullptr) {
                mediaPlayer->URL = nextPath;
                mediaPlayer->Ctlcontrols->play();
                isPlaying = true;
            }
        }

        // SpeedComboBox_SelectedIndexChanged: apply chosen playback rate and hide the combo
        // Applies the rate selected in the speed combo and then hides the
        // control to restore a compact UI.
        System::Void SpeedComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            ApplySelectedPlaybackRate();
            if (speedComboBox != nullptr) speedComboBox->Visible = false;
        }

        // SpeedComboBox_DropDownClosed: hide speed combo when dropdown closes
        System::Void SpeedComboBox_DropDownClosed(System::Object^ sender, System::EventArgs^ e) {
            if (speedComboBox != nullptr) speedComboBox->Visible = false;
        }

        // SpeedIcon_Click: open the playback rate selector
        System::Void SpeedIcon_Click(System::Object^ sender, System::EventArgs^ e) {
            if (speedComboBox == nullptr) return;
            speedComboBox->Visible = true;
            speedComboBox->Focus();
            speedComboBox->DroppedDown = true;
        }

        // ApplySelectedPlaybackRate: set mediaPlayer playback rate from combo selection
        void ApplySelectedPlaybackRate() {
            if (speedComboBox == nullptr) return;
            String^ sel = safe_cast<String^>(speedComboBox->SelectedItem);
            if (sel == nullptr) return;
            double rate = 1.0;
            if (sel->StartsWith(L"0.5")) rate = 0.5;
            else if (sel->StartsWith(L"1.0")) rate = 1.0;
            else if (sel->StartsWith(L"1.25")) rate = 1.25;
            else if (sel->StartsWith(L"1.5")) rate = 1.5;
            else if (sel->StartsWith(L"2.0")) rate = 2.0;
            mediaPlayer->settings->rate = rate;
        }

        // PreviousButton_Click: move to previous playable item (handles playlist headers)
        System::Void PreviousButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (listBox2->Items->Count == 0) return;

            int sel = listBox2->SelectedIndex;
            VideoList::Node^ node = nullptr;

            if (sel >= 0) {
                node = videoList->getNodeAtDisplayIndex(sel, listBox2);
                // if header selected, walk backwards to find previous playable
                if (node != nullptr && node->isPlaylistHeader) {
                    for (int i = sel - 1; i >= 0; --i) {
                        VideoList::Node^ n = videoList->getNodeAtDisplayIndex(i, listBox2);
                        if (n != nullptr && !n->isPlaylistHeader) { node = n; break; }
                    }
                } else if (node == nullptr) {
                    // fallback: try previousVideo() which uses internal current
                    String^ prevPath = videoList->previousVideo();
                    if (prevPath != nullptr) {
                        videoList->populateTrackList(listBox2);
                        for (int i = 0; i < listBox2->Items->Count; ++i)
                            if (videoList->getNodeAtDisplayIndex(i, listBox2) == videoList->getCurrentNode()) { listBox2->SelectedIndex = i; break; }
                        mediaPlayer->URL = prevPath;
                        if (isPlaying) mediaPlayer->Ctlcontrols->play();
                    }
                    return;
                }
            } else {
                // nothing selected — use internal previousVideo as fallback
                String^ prevPath = videoList->previousVideo();
                if (prevPath != nullptr) {
                    videoList->populateTrackList(listBox2);
                    for (int i = 0; i < listBox2->Items->Count; ++i)
                        if (videoList->getNodeAtDisplayIndex(i, listBox2) == videoList->getCurrentNode()) { listBox2->SelectedIndex = i; break; }
                    mediaPlayer->URL = prevPath;
                    if (isPlaying) mediaPlayer->Ctlcontrols->play();
                }
                return;
            }

            if (node != nullptr) {
                videoList->setCurrentNode(node);
                videoList->populateTrackList(listBox2);
                for (int i = 0; i < listBox2->Items->Count; ++i)
                    if (videoList->getNodeAtDisplayIndex(i, listBox2) == node) { listBox2->SelectedIndex = i; break; }
                String^ path = videoList->getCurrentNodePath();
                if (path != nullptr) { mediaPlayer->URL = path; if (isPlaying) mediaPlayer->Ctlcontrols->play(); }
            }
        }

        // ResetButton_Click: reset playlist selection to first playable video
        System::Void ResetButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (listBox2->Items->Count == 0) return;
            // Ensure we pick the first playable video (skip any playlist headers)
            auto first = videoList->getFirstPlayableNode();
            if (first == nullptr) return;
            videoList->setCurrentNode(first);
            // update list selection to the matching display index
            listBox2->BeginUpdate();
            for (int i = 0; i < listBox2->Items->Count; i++) {
                VideoList::Node^ n = videoList->getNodeAtDisplayIndex(i, listBox2);
                if (n == first) { listBox2->SelectedIndex = i; break; }
            }
            listBox2->EndUpdate();

            String^ path = videoList->getCurrentNodePath();
            if (path != nullptr) {
                mediaPlayer->URL = path;
            }
            MessageBox::Show("Playlist reset to first video!", "Reset",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        // PositionTrackBar_MouseDown: mark that user started dragging the position slider
        System::Void PositionTrackBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            isDraggingSlider = true;
        }

        // PositionTrackBar_MouseUp: apply new playback position when user releases the slider
        System::Void PositionTrackBar_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            isDraggingSlider = false;
            if (mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        // PositionTrackBar_Scroll: update playback position programmatically when slider moved
        System::Void PositionTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e) {
            if (!isDraggingSlider && mediaPlayer->currentMedia != nullptr) {
                double duration = mediaPlayer->currentMedia->duration;
                double newPosition = (positionTrackBar->Value / 100.0) * duration;
                mediaPlayer->Ctlcontrols->currentPosition = newPosition;
            }
        }

        // VolumeTrackBar_Scroll: sync mediaPlayer volume with the UI slider
        System::Void VolumeTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e) {
            mediaPlayer->settings->volume = volumeTrackBar->Value;
        }

        // Timer_Tick: periodically update position slider and time label while playing
        System::Void Timer_Tick(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia != nullptr && !isDraggingSlider) {
                double duration = mediaPlayer->currentMedia->duration;
                double position = mediaPlayer->Ctlcontrols->currentPosition;

                if (duration > 0) {
                    int percentage = (int)((position / duration) * 100);
                    positionTrackBar->Value = Math::Min(percentage, 100);
                }

                timeLabel->Text = FormatTime(position) + " / " + FormatTime(duration);
            }
        }

        // SkipForwardButton_Click: seek forward a few seconds
        System::Void SkipForwardButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double duration = mediaPlayer->currentMedia->duration;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Min(duration, pos + 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        // SkipBackwardButton_Click: seek backward a few seconds
        System::Void SkipBackwardButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (mediaPlayer->currentMedia == nullptr) return;
            double pos = mediaPlayer->Ctlcontrols->currentPosition;
            double newPos = Math::Max(0.0, pos - 5.0);
            mediaPlayer->Ctlcontrols->currentPosition = newPos;
        }

        // LoopButton_Click: toggle playlist looping and update visual state
        System::Void LoopButton_Click(System::Object^ sender, System::EventArgs^ e) {
            // Toggle playlist looping on/off. Single-item loop removed.
            loopEnabled = !loopEnabled;
            // visual feedback via background color
            if (loopEnabled) loopButton->BackColor = System::Drawing::Color::FromArgb(52, 152, 219);
            else loopButton->BackColor = System::Drawing::Color::Gray;
            // update image for on/off state
            UpdateLoopButtonImage();
        }

        // FullScreenButton_Click: toggle fullscreen mode and rearrange controls
        System::Void FullScreenButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!isFullscreen) {
                // save current window state and bounds so we can restore after exiting fullscreen
                this->previousWindowState = this->WindowState;
                this->previousBorderStyle = this->FormBorderStyle;
                this->previousBounds = this->Bounds;

                // allow maximizing by clearing fixed size constraints
                this->MaximumSize = System::Drawing::Size(0,0);
                this->MinimumSize = System::Drawing::Size(0,0);

                this->WindowState = FormWindowState::Maximized;
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
                // Make the video responsive by docking the media player to fill the panel
                videoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::Fill; // let the control resize automatically
                // allow media player to scale the video to the available area in fullscreen
                mediaPlayer->stretchToFit = true;
                controlPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
                videoPanel->BringToFront();
                controlPanel->BringToFront();
                // keep control panel visible in fullscreen; optionally preserve original positions/sizes
                controlPanel->Visible = true;
                // always use ArrangeControlsCentered which now follows requested layout
                this->ArrangeControlsCentered();
                if (useOriginalFullscreenPositions) {
                    // restore original sizes and positions for fullscreen controls, scaled to current controlPanel size
                    double sx = 1.0;
                    double sy = 1.0;
                    if (this->originalControlPanelSize.Width > 0) sx = (double)controlPanel->ClientSize.Width / this->originalControlPanelSize.Width;
                    if (this->originalControlPanelSize.Height > 0) sy = (double)controlPanel->ClientSize.Height / this->originalControlPanelSize.Height;

                    // apply scaled positions/sizes
                    for each (System::String^ key in this->originalPositions->Keys)
                    {
                        System::Drawing::Point p = this->originalPositions[key];
                        p.X = (int)(p.X * sx);
                        p.Y = (int)(p.Y * sy);
                        // set by key
                        if (key->Equals(L"playButton")) this->playButton->Location = p;
                        else if (key->Equals(L"pauseButton")) this->pauseButton->Location = p;
                        else if (key->Equals(L"previousButton")) this->previousButton->Location = p;
                        else if (key->Equals(L"skipBackwardButton")) this->skipBackwardButton->Location = p;
                        else if (key->Equals(L"skipForwardButton")) this->skipForwardButton->Location = p;
                        else if (key->Equals(L"nextButton")) this->nextButton->Location = p;
                        else if (key->Equals(L"resetButton")) this->resetButton->Location = p;
                        else if (key->Equals(L"shuffleButton")) this->shuffleButton->Location = p;
                        else if (key->Equals(L"loopButton")) this->loopButton->Location = p;
                        else if (key->Equals(L"volumeTrackBar")) this->volumeTrackBar->Location = p;
                        else if (key->Equals(L"speedComboBox")) this->speedComboBox->Location = p;
                        else if (key->Equals(L"positionTrackBar")) this->positionTrackBar->Location = p;
                        else if (key->Equals(L"timeLabel")) this->timeLabel->Location = p;
                        else if (key->Equals(L"volumeIcon")) this->volumeIcon->Location = p;
                        else if (key->Equals(L"speedIcon")) this->speedIcon->Location = p;
                    }

                    for each (System::String^ key in this->originalSizes->Keys)
                    {
                        System::Drawing::Size s = this->originalSizes[key];
                        s.Width = (int)(s.Width * sx);
                        s.Height = (int)(s.Height * sy);
                        if (key->Equals(L"playButton")) this->playButton->Size = s;
                        else if (key->Equals(L"pauseButton")) this->pauseButton->Size = s;
                        else if (key->Equals(L"previousButton")) this->previousButton->Size = s;
                        else if (key->Equals(L"skipBackwardButton")) this->skipBackwardButton->Size = s;
                        else if (key->Equals(L"skipForwardButton")) this->skipForwardButton->Size = s;
                        else if (key->Equals(L"nextButton")) this->nextButton->Size = s;
                        else if (key->Equals(L"resetButton")) this->resetButton->Size = s;
                        else if (key->Equals(L"shuffleButton")) this->shuffleButton->Size = s;
                        else if (key->Equals(L"loopButton")) this->loopButton->Size = s;
                        else if (key->Equals(L"volumeTrackBar")) this->volumeTrackBar->Size = s;
                        else if (key->Equals(L"speedComboBox")) this->speedComboBox->Size = s;
                        else if (key->Equals(L"positionTrackBar")) this->positionTrackBar->Size = s;
                        else if (key->Equals(L"timeLabel")) this->timeLabel->Size = s;
                        else if (key->Equals(L"volumeIcon")) this->volumeIcon->Size = s;
                        else if (key->Equals(L"speedIcon")) this->speedIcon->Size = s;
                    }
                } else {
                    this->ArrangeControlsCentered();
                }
                isFullscreen = true;
                // swap fullscreen button image to exit image if available
                try {
                    String^ exitImg = Path::Combine(Application::StartupPath, "Images\\btnexitScreen.png");
                    if (File::Exists(exitImg)) {
                        fullScreenButton->BackgroundImage = Image::FromFile(exitImg);
                        fullScreenButton->BackgroundImageLayout = ImageLayout::Stretch;
                    }
                } catch (Exception^) { }
                // When docked Fill the control will resize with the panel; still call Adjust to ensure correct placement
                this->AdjustMediaPlayerAspect();
                // exit fullscreen button removed
            }
            else {
                // restore to non-resizable normal window (do not allow user stretching)
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
                this->WindowState = FormWindowState::Normal;
                // restore previous window bounds
                try { this->Bounds = this->previousBounds; } catch (Exception^) { }
                // re-apply fixed size limits to prevent stretching
                this->MaximumSize = this->Size;
                this->MinimumSize = this->Size;
                controlPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
                videoPanel->Dock = System::Windows::Forms::DockStyle::None;
                // Restore previous layout: undock media player and let PositionMediaPlayer compute size
                mediaPlayer->Dock = System::Windows::Forms::DockStyle::None;
                // restore stretchToFit to true so the player behaves like designer layout
                mediaPlayer->stretchToFit = true;
                // exit fullscreen button removed
                PositionMediaPlayer();
                isFullscreen = false;
                // undock controlPanel and position it immediately below the videoPanel
                try {
                    controlPanel->Dock = System::Windows::Forms::DockStyle::None;
                    int left = videoPanel->Left;
                    int top = videoPanel->Bottom;
                    int width = videoPanel->Width;
                    int height = controlPanel->Height;
                    // clamp so controlPanel stays inside the form
                    if (top + height > this->ClientSize.Height) {
                        top = Math::Max(0, this->ClientSize.Height - height - 10);
                    }
                    if (left + width > this->ClientSize.Width) {
                        width = Math::Max(100, this->ClientSize.Width - left - 10);
                    }
                    controlPanel->Location = System::Drawing::Point(left, top);
                    controlPanel->Width = width;
                } catch (Exception^) { }
                controlPanel->Visible = true;
                controlPanel->BringToFront();
                // ensure controlPanel layout updated so ArrangeControlsCentered computes correct positions
                try {
                    controlPanel->Refresh();
                    controlPanel->PerformLayout();
                } catch (Exception^) { }
                // Restore original designer positions. Do not re-run ArrangeControlsCentered()
                // after restoring - that can recompute positions and cause overlap.
                RestoreOriginalControlPositions();
                // center the time label within the control panel to ensure it's centered after exiting fullscreen
                try {
                    if (this->controlPanel != nullptr && this->timeLabel != nullptr && this->positionTrackBar != nullptr) {
                        int panelW = this->controlPanel->ClientSize.Width;
                        int x = (panelW - this->timeLabel->Width) / 2;
                        int y = this->positionTrackBar->Bottom + 4;
                        this->timeLabel->Location = System::Drawing::Point(System::Math::Max(0, x), System::Math::Max(0, y));
                    }
                } catch (Exception^) { }

                // Ensure fullscreen button is visible and within bounds after restoring
                try {
                    if (fullScreenButton != nullptr) {
                        fullScreenButton->Visible = true;
                        // clamp to control panel width
                        int maxLeft = System::Math::Max(10, controlPanel->ClientSize.Width - fullScreenButton->Width - 10);
                        if (fullScreenButton->Left > maxLeft) fullScreenButton->Left = maxLeft;
                        if (fullScreenButton->Left < 10) fullScreenButton->Left = 10;
                        fullScreenButton->BringToFront();
                        controlPanel->BringToFront();

                        // restore fullscreen button image back to fullscreen icon if available
                        try {
                            String^ fullImg = Path::Combine(Application::StartupPath, "Images\\btnfullscreen.png");
                            if (File::Exists(fullImg)) {
                                fullScreenButton->BackgroundImage = Image::FromFile(fullImg);
                                fullScreenButton->BackgroundImageLayout = ImageLayout::Stretch;
                            }
                        } catch (Exception^) { }
                    }
                } catch (Exception^) { }

            }
        }

        

        // VideoPanel_MouseMove: show/hide control panel when mouse near bottom in fullscreen
        System::Void VideoPanel_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            if (!isFullscreen) return;
            int threshold = 120;
            int distFromBottom = videoPanel->ClientSize.Height - e->Y;
            controlPanel->Visible = distFromBottom <= threshold;
        }

        // Wrapper handler used to avoid duplicate symbol with designer-generated method
        // VideoPanel_Resize_Handler: wrapper to adjust media player aspect when panel resizes
        System::Void VideoPanel_Resize_Handler(System::Object^ sender, System::EventArgs^ e) {
            try {
                this->AdjustMediaPlayerAspect();
            }
            catch (Exception^) {
                // ignore
            }
        }

        // ControlPanel_Resize: re-arrange controls when the control panel size changes
        System::Void ControlPanel_Resize(System::Object^ sender, System::EventArgs^ e) {
            ArrangeControlsCentered();
        }

        // ShuffleButton_Click: pick a random playlist item and play it
        System::Void ShuffleButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (listBox2->Items->Count > 0) {
                System::Random^ rand = gcnew System::Random();
                int currentIdx = listBox2->SelectedIndex;
                int idx = rand->Next(0, listBox2->Items->Count);
                if (listBox2->Items->Count > 1 && idx == currentIdx) {
                    idx = (idx + 1) % listBox2->Items->Count;
                }
                listBox2->SelectedIndex = idx;
                videoList->setCurrentNode(idx);
                String^ path = videoList->getCurrentNodePath();
                if (path != nullptr) {
                    mediaPlayer->URL = path;
                    mediaPlayer->Ctlcontrols->play();
                    isPlaying = true;
                    controlPanel->Visible = true;
                    ShowVideoPanel();
                }
            }
        }

        // FormatTime: convert seconds to MM:SS string
        String^ FormatTime(double seconds) {
            int min = (int)(seconds / 60);
            int sec = (int)seconds % 60;
            return String::Format(L"{0:D2}:{1:D2}", min, sec);
        }

        // CaptureOriginalControlPositions: remember control positions/sizes for restoring
        void CaptureOriginalControlPositions() {
            this->originalPositions = gcnew System::Collections::Generic::Dictionary<System::String^, System::Drawing::Point>();
            this->originalSizes = gcnew System::Collections::Generic::Dictionary<System::String^, System::Drawing::Size>();
            this->AddPos(L"previousButton", this->previousButton);
            this->AddPos(L"skipBackwardButton", this->skipBackwardButton);
            this->AddPos(L"playButton", this->playButton);
            this->AddPos(L"pauseButton", this->pauseButton);
            this->AddPos(L"skipForwardButton", this->skipForwardButton);
            this->AddPos(L"nextButton", this->nextButton);
            this->AddPos(L"resetButton", this->resetButton);
            this->AddPos(L"fullScreenButton", this->fullScreenButton);
            this->AddPos(L"shuffleButton", this->shuffleButton);
            this->AddPos(L"loopButton", this->loopButton);
            this->AddPos(L"volumeTrackBar", this->volumeTrackBar);
            this->AddPos(L"speedComboBox", this->speedComboBox);
            this->AddPos(L"positionTrackBar", this->positionTrackBar);
            this->AddPos(L"timeLabel", this->timeLabel);
            this->AddPos(L"volumeIcon", this->volumeIcon);
            this->AddPos(L"speedIcon", this->speedIcon);
            // capture sizes
            this->AddSize(L"previousButton", this->previousButton);
            this->AddSize(L"skipBackwardButton", this->skipBackwardButton);
            this->AddSize(L"playButton", this->playButton);
            this->AddSize(L"pauseButton", this->pauseButton);
            this->AddSize(L"skipForwardButton", this->skipForwardButton);
            this->AddSize(L"nextButton", this->nextButton);
            this->AddSize(L"resetButton", this->resetButton);
            this->AddSize(L"fullScreenButton", this->fullScreenButton);
            this->AddSize(L"shuffleButton", this->shuffleButton);
            this->AddSize(L"loopButton", this->loopButton);
            this->AddSize(L"volumeTrackBar", this->volumeTrackBar);
            this->AddSize(L"speedComboBox", this->speedComboBox);
            this->AddSize(L"positionTrackBar", this->positionTrackBar);
            this->AddSize(L"timeLabel", this->timeLabel);
            this->AddSize(L"volumeIcon", this->volumeIcon);
            this->AddSize(L"speedIcon", this->speedIcon);
            // capture original controlPanel size for proportional scaling
            if (this->controlPanel != nullptr) this->originalControlPanelSize = this->controlPanel->Size;
            // attach hover handlers to designer controls to mimic new style
            array<System::Windows::Forms::Control^>^ hoverControls = gcnew array<System::Windows::Forms::Control^>(9);
            hoverControls[0] = playButton;
            hoverControls[1] = pauseButton;
            hoverControls[2] = previousButton;
            hoverControls[3] = nextButton;
            hoverControls[4] = resetButton;
            hoverControls[5] = fullScreenButton;
            hoverControls[6] = skipForwardButton;
            hoverControls[7] = skipBackwardButton;
            hoverControls[8] = shuffleButton;
            for each (Control^ c in hoverControls) {
                if (c != nullptr) {
                    c->MouseEnter += gcnew System::EventHandler(this, &MainForm::Button_MouseEnter);
                    c->MouseLeave += gcnew System::EventHandler(this, &MainForm::Button_MouseLeave);
                }
            }
        }

        // RestoreOriginalControlPositions: restore captured control positions/sizes
        void RestoreOriginalControlPositions() {
            if (this->originalPositions == nullptr) return;
            this->SetPos(L"previousButton", this->previousButton);
            this->SetPos(L"skipBackwardButton", this->skipBackwardButton);
            if (this->playButton != nullptr) this->SetPos(L"playButton", this->playButton);
            if (this->pauseButton != nullptr) this->SetPos(L"pauseButton", this->pauseButton);
            this->SetPos(L"skipForwardButton", this->skipForwardButton);
            this->SetPos(L"nextButton", this->nextButton);
            this->SetPos(L"resetButton", this->resetButton);
            this->SetPos(L"fullScreenButton", this->fullScreenButton);
            this->SetPos(L"shuffleButton", this->shuffleButton);
            this->SetPos(L"loopButton", this->loopButton);
            this->SetPos(L"volumeTrackBar", this->volumeTrackBar);
            this->SetPos(L"speedComboBox", this->speedComboBox);
            this->SetPos(L"positionTrackBar", this->positionTrackBar);
            this->SetPos(L"timeLabel", this->timeLabel);
            this->SetPos(L"volumeIcon", this->volumeIcon);
            this->SetPos(L"speedIcon", this->speedIcon);
            // restore sizes if captured
            if (this->originalSizes != nullptr) {
                this->SetSize(L"previousButton", this->previousButton);
                this->SetSize(L"skipBackwardButton", this->skipBackwardButton);
                this->SetSize(L"playButton", this->playButton);
                this->SetSize(L"pauseButton", this->pauseButton);
                this->SetSize(L"skipForwardButton", this->skipForwardButton);
                this->SetSize(L"nextButton", this->nextButton);
                this->SetSize(L"resetButton", this->resetButton);
                this->SetSize(L"fullScreenButton", this->fullScreenButton);
                this->SetSize(L"shuffleButton", this->shuffleButton);
                this->SetSize(L"loopButton", this->loopButton);
                this->SetSize(L"volumeTrackBar", this->volumeTrackBar);
                this->SetSize(L"speedComboBox", this->speedComboBox);
                this->SetSize(L"positionTrackBar", this->positionTrackBar);
                this->SetSize(L"timeLabel", this->timeLabel);
                this->SetSize(L"volumeIcon", this->volumeIcon);
                this->SetSize(L"speedIcon", this->speedComboBox);
            }
        }

        // AddPos: record control location under given key
        void AddPos(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalPositions != nullptr && !this->originalPositions->ContainsKey(key)) {
                this->originalPositions->Add(key, c->Location);
            }
        }

        // AddSize: record control size under given key
        void AddSize(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalSizes != nullptr && !this->originalSizes->ContainsKey(key)) {
                this->originalSizes->Add(key, c->Size);
            }
        }

        // SetSize: apply previously recorded size to control
        void SetSize(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalSizes != nullptr && this->originalSizes->ContainsKey(key)) {
                c->Size = this->originalSizes[key];
            }
        }

        // SetPos: apply previously recorded position to control
        void SetPos(System::String^ key, System::Windows::Forms::Control^ c) {
            if (c != nullptr && this->originalPositions != nullptr && this->originalPositions->ContainsKey(key)) {
                c->Location = this->originalPositions[key];
            }
        }

        // hover handlers copied from sample
        // Button_MouseEnter: hover visual feedback for buttons
        void Button_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
            Control^ ctrl = safe_cast<Control^>(sender);
            ctrl->BackColor = System::Drawing::Color::FromArgb(80, 120, 160);
        }

        // Button_MouseLeave: remove hover visual feedback for buttons
        void Button_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
            Control^ ctrl = safe_cast<Control^>(sender);
            ctrl->BackColor = System::Drawing::Color::FromArgb(60, 100, 140);
        }
    };
}