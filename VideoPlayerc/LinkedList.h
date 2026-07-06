#pragma once
// Minimal includes — implementation uses .NET types, avoid unnecessary STL headers in the header
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;


ref class VideoList
{
public:
    ref class Node
    {
    public:
        String^ videoPath;
        String^ videoName;
        String^ playlistName;
        bool isPlaylistHeader;
        bool isExpanded;
        DateTime addedTime;
        Node^ next;

            // Node constructor: initialize node metadata. If isHeader is true
            // the node represents a playlist header rather than a single video.
        Node(String^ path, String^ name, String^ playlist, bool isHeader);
    };

private:
    Node^ head;
    Node^ tail;
    Node^ current;
    int size;

        // appendNode: internal helper to attach a node at the end of the list
    void appendNode(Node^ node);
        // isDuplicate: returns true when a video path already exists in list
    bool isDuplicate(String^ path);
        // getVideoAt: return the node at the provided zero-based index
    Node^ getVideoAt(int index);
        // getAllPlayableVideos: return a linear list of non-header video nodes
    List<Node^>^ getAllPlayableVideos();

public:
    VideoList();

    // Basic operations
    
    // isEmpty: returns true when list contains no nodes
    bool isEmpty();
    // getSize: returns number of nodes currently in list
    int getSize();

    // Add operations
    // addPlaylist: create playlist header and append given videos
    void addPlaylist(array<String^>^ videoPaths, String^ playlistName, ListBox^ box);
    // addVideo: append a single video node
    void addVideo(String^ path, String^ name, ListBox^ box);

    // Playlist operations
    
    // togglePlaylistExpansion: toggle expanded/collapsed state of a playlist
    void togglePlaylistExpansion(int index, ListBox^ box);
    // getPlaylistVideos: return all videos that belong to a named playlist
    List<Node^>^ getPlaylistVideos(String^ playlistName);

    // Display operations
    // populateTrackList: populate a ListBox with the current list state
    void populateTrackList(ListBox^ box);
    // getNodeAtDisplayIndex: map ListBox display index to underlying node
    Node^ getNodeAtDisplayIndex(int displayIndex, ListBox^ box);

    // Playback operations
    // nextVideo: advance to next playable video and return its path
    String^ nextVideo();
    // previousVideo: move to previous playable video and return path
    String^ previousVideo();
    // getRandomVideo: pick a random playable video and return its path
    String^ getRandomVideo();

    // Current node operations
    // getCurrentNodePath: returns video path of current node
    String^ getCurrentNodePath();
    // getCurrentNodeName: returns display name of current node
    String^ getCurrentNodeName();
    // setCurrentNode: set current node by pointer
    void setCurrentNode(Node^ node);
    // setCurrentNode(index): set current by absolute list index
    void setCurrentNode(int index);
    // getCurrentNode: get pointer to current node
    Node^ getCurrentNode();
    // getCurrentNodeIndex: return index of current or -1
    int getCurrentNodeIndex();
    // getFirstPlayableNode: returns first non-header video node
    Node^ getFirstPlayableNode();

    // Remove operations
    // removeItem: remove a displayed item (playlist or video)
    void removeItem(int displayIndex, ListBox^ box);
    // clearAll: remove all playlists and videos after confirmation
    void clearAll(ListBox^ box);
    // File operations
    // saveToFile: persist list to a textual save file
    void saveToFile(String^ filename, ListBox^ box);
    // loadFromFile: read list from a textual save file
    void loadFromFile(String^ filename, ListBox^ box);

    // Search operations
    // searchVideos: return nodes whose names contain the search term
    List<Node^>^ searchVideos(String^ searchTerm);

    // Sorting operations
    
    // sortAlphabetically: sort items A-Z preserving playlist grouping
    void sortAlphabetically(ListBox^ box);
    // sortByTimeAdded: sort items by their addedTime (oldest first)
    void sortByTimeAdded(ListBox^ box);

    // Utility operations
    
    // shuffle: pick a random playable node and return its display index
    int shuffle();
    // printList: debug helper to print video names to console
    void printList();
};