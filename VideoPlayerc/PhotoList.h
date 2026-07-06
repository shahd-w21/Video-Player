#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace VideoPlayerc {

    ref class PhotoList {
public:
    ref class Node {
    public:
        String^ photoName;
        String^ photoPath;
        DateTime addedTime;
        String^ albumName;
        bool isAlbumHeader;
        bool isExpanded;
    };

    ref class NodeArray {
    public:
        List<Node^>^ items;
        NodeArray() { items = gcnew List<Node^>(); }
        property int count { int get() { return items->Count; } }
        Node^ get(int i) { return (i >= 0 && i < items->Count) ? items[i] : nullptr; }
    };

    // Constructor: initialize an empty photo collection
    PhotoList();

    // Display helpers
    // populatePhotoList: fill a ListBox with albums and photos (handles expansion)
    void populatePhotoList(ListBox^ lb);
    // getCurrentNode: return the current selected photo node
    Node^ getCurrentNode();
    // getCurrentNodePath: return filesystem path of current node or nullptr
    String^ getCurrentNodePath();
    // getCurrentNodeName: return display name of current node or nullptr
    String^ getCurrentNodeName();

    // Add operations
    // addPhoto: append a single photo to the collection and refresh UI
    void addPhoto(String^ path, String^ name, ListBox^ lb);
    // addAlbum: add an album header and associated photos
    void addAlbum(array<String^>^ paths, String^ albumName, ListBox^ lb);

    // Remove and clear operations
    // removeItem: remove an item by its displayed index (handles album removal)
    void removeItem(int displayIndex, ListBox^ lb);
    // clearAll: remove all photos and albums
    void clearAll(ListBox^ lb);

    // Persistence
    // saveToFile: write photo list to disk
    void saveToFile(String^ filename, ListBox^ lb);
    // loadFromFile: read photo list from disk and populate UI
    void loadFromFile(String^ filename, ListBox^ lb);

    // Sorting
    // sortAlphabetically: sort photos by name
    void sortAlphabetically(ListBox^ lb);
    // sortByTimeAdded: sort photos by added time
    void sortByTimeAdded(ListBox^ lb);

    // Navigation
    // previousPhoto: move to previous non-header photo and return its path
    String^ previousPhoto();
    // nextPhoto: move to next non-header photo and return its path
    String^ nextPhoto();
    // shuffle: pick a random photo index (excluding headers)
    int shuffle();
    // isEmpty: return true when no photos are present
    bool isEmpty();

    // Album operations
    // toggleAlbumExpansion: expand/collapse an album at display index
    void toggleAlbumExpansion(int displayIndex, ListBox^ lb);

    // Current node management and search
    // setCurrentNode: set the current node pointer
    void setCurrentNode(Node^ node);
    // getNodeAtDisplayIndex: map a display index to an underlying node
    Node^ getNodeAtDisplayIndex(int displayIndex, ListBox^ lb);
    // searchPhotos: returns a NodeArray of matches for the search term
    NodeArray^ searchPhotos(String^ term);

    private:
        List<Node^>^ nodes;
        // track currently selected photo without reordering the list
        Node^ currentNode;
    };

}
