#include "PhotoList.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace VideoPlayerc {

    PhotoList::PhotoList()
    {
        // Constructor: initialize internal list and reset current node
        nodes = gcnew List<Node^>();
        currentNode = nullptr;
    }

    static PhotoList::Node^ CreatePhotoNode(String^ path, String^ name)
    {
        // CreatePhotoNode: helper to build a PhotoList::Node with metadata
        PhotoList::Node^ n = gcnew PhotoList::Node();
        n->photoPath = path;
        n->photoName = name;
        n->albumName = nullptr;
        n->isAlbumHeader = false;
        n->isExpanded = false;
        n->addedTime = DateTime::Now;
        return n;
    }

    PhotoList::NodeArray^ PhotoList::searchPhotos(String^ term)
    {
        // searchPhotos: case-insensitive substring match against photo names
        PhotoList::NodeArray^ results = gcnew PhotoList::NodeArray();
        if (String::IsNullOrWhiteSpace(term)) return results;
        String^ low = term->ToLower();
        for each (Node^ n in nodes)
        {
            if (n == nullptr || n->isAlbumHeader) continue;
            if (n->photoName != nullptr && n->photoName->ToLower()->Contains(low)) results->items->Add(n);
        }
        return results;
    }

    void PhotoList::populatePhotoList(ListBox^ lb)
    {
        // populatePhotoList: render albums and photos into a ListBox taking
        // into account album expansion state.
        if (lb == nullptr) return;
        lb->Items->Clear();
        for each (Node^ n in nodes)
        {
            if (n == nullptr) continue;
            if (n->isAlbumHeader)
            {
                String^ display = "[Album] " + n->albumName;
                lb->Items->Add(display);
                if (n->isExpanded)
                {
                    int startIdx = nodes->IndexOf(n) + 1;
                    for (int i = startIdx; i < nodes->Count; i++)
                    {
                        Node^ c = nodes[i];
                        if (c == nullptr) continue;
                        if (c->isAlbumHeader) break;
                        if (c->albumName == n->albumName)
                            lb->Items->Add("  " + c->photoName);
                    }
                }
            }
            else if (n->albumName == nullptr)
            {
                lb->Items->Add(n->photoName);
            }
        }
    }

    PhotoList::Node^ PhotoList::getCurrentNode()
    {
        // getCurrentNode: return currently selected node pointer
        return currentNode;
    }

    String^ PhotoList::getCurrentNodePath()
    {
        // getCurrentNodePath: return filesystem path of the current node
        Node^ n = getCurrentNode();
        return n == nullptr ? nullptr : n->photoPath;
    }

    String^ PhotoList::getCurrentNodeName()
    {
        // getCurrentNodeName: return the display name of the current node
        Node^ n = getCurrentNode();
        return n == nullptr ? nullptr : n->photoName;
    }

    void PhotoList::addPhoto(String^ path, String^ name, ListBox^ lb)
    {
        // addPhoto: adds a photo node if it does not already exist
        if (String::IsNullOrWhiteSpace(path)) return;
        for each (Node^ n in nodes) if (n->photoPath == path) return;
        Node^ n = CreatePhotoNode(path, name);
        nodes->Add(n);
        if (lb != nullptr) populatePhotoList(lb);
    }

    void PhotoList::addAlbum(array<String^>^ paths, String^ albumName, ListBox^ lb)
    {
        // addAlbum: create a header node and append provided photo nodes
        if (String::IsNullOrWhiteSpace(albumName) || paths == nullptr || paths->Length == 0) return;
        Node^ header = gcnew Node();
        header->isAlbumHeader = true;
        header->albumName = albumName;
        header->photoName = nullptr;
        header->photoPath = nullptr;
        header->isExpanded = true;
        header->addedTime = DateTime::Now;
        nodes->Add(header);

        for each (String^ p in paths)
        {
            String^ n = Path::GetFileName(p);
            Node^ item = CreatePhotoNode(p, n);
            item->albumName = albumName;
            nodes->Add(item);
        }

        if (lb != nullptr) populatePhotoList(lb);
    }

    void PhotoList::removeItem(int displayIndex, ListBox^ lb)
    {
        // removeItem: remove a displayed photo or an entire album by
        // mapping the display index back into the nodes list.
        if (lb == nullptr) return;
        if (displayIndex < 0 || displayIndex >= lb->Items->Count) return;

        int idx = 0;
        for (int i = 0; i < nodes->Count; i++)
        {
            Node^ n = nodes[i];
            if (n == nullptr) continue;
            if (n->isAlbumHeader)
            {
                if (displayIndex == idx) {
                    String^ album = n->albumName;
                    List<Node^>^ toRemove = gcnew List<Node^>();
                    toRemove->Add(n);
                    int j = i + 1;
                    while (j < nodes->Count && !nodes[j]->isAlbumHeader && nodes[j]->albumName == album)
                    {
                        toRemove->Add(nodes[j]); j++;
                    }
                    for each (Node^ r in toRemove) nodes->Remove(r);
                    if (lb != nullptr) populatePhotoList(lb);
                    return;
                }
                idx++;
                if (n->isExpanded)
                {
                    int j = i + 1;
                    while (j < nodes->Count && !nodes[j]->isAlbumHeader && nodes[j]->albumName == n->albumName)
                    {
                        if (displayIndex == idx) { nodes->RemoveAt(j); if (lb != nullptr) populatePhotoList(lb); return; }
                        idx++; j++;
                    }
                }
            }
            else if (n->albumName == nullptr)
            {
                if (displayIndex == idx) { nodes->RemoveAt(i); if (lb != nullptr) populatePhotoList(lb); return; }
                idx++;
            }
        }
    }

    void PhotoList::clearAll(ListBox^ lb)
    {
        // clearAll: remove all nodes and clear the provided ListBox UI
        nodes->Clear();
        if (lb != nullptr) lb->Items->Clear();
    }

    void PhotoList::saveToFile(String^ filename, ListBox^ lb)
    {
        // saveToFile: serialize nodes to a simple pipe-delimited text file
        try {
            StreamWriter^ w = gcnew StreamWriter(filename);
            for each (Node^ n in nodes)
            {
                if (n->isAlbumHeader)
                {
                    w->WriteLine("H|" + n->albumName);
                }
                else
                {
                    String^ album = n->albumName == nullptr ? "" : n->albumName;
                    w->WriteLine("P|" + n->photoPath + "|" + n->photoName + "|" + album + "|" + n->addedTime.ToFileTime().ToString());
                }
            }
            w->Close();
        }
        catch (...) {}
    }

    void PhotoList::loadFromFile(String^ filename, ListBox^ lb)
    {
        // loadFromFile: read stored nodes back and rebuild the nodes list
        try {
            if (!File::Exists(filename)) return;
            StreamReader^ r = gcnew StreamReader(filename);
            nodes->Clear();
            while (!r->EndOfStream)
            {
                String^ line = r->ReadLine();
                if (String::IsNullOrWhiteSpace(line)) continue;
                array<String^>^ parts = line->Split('|');
                if (parts->Length == 0) continue;
                if (parts[0] == "H")
                {
                    Node^ h = gcnew Node();
                    h->isAlbumHeader = true;
                    h->albumName = parts->Length > 1 ? parts[1] : "";
                    h->isExpanded = false;
                    nodes->Add(h);
                }
                else if (parts[0] == "P")
                {
                    String^ path = parts->Length > 1 ? parts[1] : "";
                    String^ name = parts->Length > 2 ? parts[2] : Path::GetFileName(path);
                    String^ album = parts->Length > 3 ? parts[3] : nullptr;
                    Node^ n = CreatePhotoNode(path, name);
                    n->albumName = String::IsNullOrEmpty(album) ? nullptr : album;
                    if (parts->Length > 4) {
                        try { n->addedTime = DateTime::FromFileTime(Int64::Parse(parts[4])); } catch(...) {}
                    }
                    nodes->Add(n);
                }
            }
            r->Close();
            if (lb != nullptr) populatePhotoList(lb);
        }
        catch (...) {}
    }

    void PhotoList::sortAlphabetically(ListBox^ lb)
    {
        // sortAlphabetically: sort photos by name (ignores album headers)
        List<Node^>^ flat = gcnew List<Node^>();
        for each (Node^ n in nodes) if (!n->isAlbumHeader) flat->Add(n);
        for (int i = 0; i < flat->Count - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < flat->Count; j++) {
                if (String::Compare(flat[j]->photoName, flat[minIdx]->photoName, true) < 0) minIdx = j;
            }
            if (minIdx != i) {
                Node^ tmp = flat[i]; flat[i] = flat[minIdx]; flat[minIdx] = tmp;
            }
        }
        nodes->Clear();
        for each (Node^ n in flat) nodes->Add(n);
        if (lb != nullptr) populatePhotoList(lb);
    }

    void PhotoList::sortByTimeAdded(ListBox^ lb)
    {
        // sortByTimeAdded: sort photos by their addedTime field (oldest first)
        List<Node^>^ flat = gcnew List<Node^>();
        for each (Node^ n in nodes) if (!n->isAlbumHeader) flat->Add(n);
        for (int i = 0; i < flat->Count - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < flat->Count; j++) {
                if (DateTime::Compare(flat[j]->addedTime, flat[minIdx]->addedTime) < 0) minIdx = j;
            }
            if (minIdx != i) {
                Node^ tmp = flat[i]; flat[i] = flat[minIdx]; flat[minIdx] = tmp;
            }
        }
        nodes->Clear();
        for each (Node^ n in flat) nodes->Add(n);
        if (lb != nullptr) populatePhotoList(lb);
    }

    String^ PhotoList::previousPhoto()
    {
        // previousPhoto: return path of previous non-header photo relative
        // to current and set it as current
        if (nodes == nullptr || nodes->Count == 0) return nullptr;
        if (currentNode == nullptr)
        {
            // pick first non-header as current if none
            for (int i = 0; i < nodes->Count; i++) if (!nodes[i]->isAlbumHeader) { currentNode = nodes[i]; break; }
            if (currentNode == nullptr) return nullptr;
        }

        // find index of currentNode
        int curIndex = -1;
        for (int i = 0; i < nodes->Count; i++) if (nodes[i] == currentNode) { curIndex = i; break; }
        if (curIndex == -1) return nullptr;

        for (int j = curIndex - 1; j >= 0; j--) {
            if (!nodes[j]->isAlbumHeader) {
                setCurrentNode(nodes[j]);
                return nodes[j]->photoPath;
            }
        }
        return nullptr;
    }

    String^ PhotoList::nextPhoto()
    {
        // nextPhoto: return path of next non-header photo relative to
        // current and set it as current
        if (nodes == nullptr || nodes->Count == 0) return nullptr;
        if (currentNode == nullptr)
        {
            for (int i = 0; i < nodes->Count; i++) if (!nodes[i]->isAlbumHeader) { currentNode = nodes[i]; break; }
            if (currentNode == nullptr) return nullptr;
        }

        int curIndex = -1;
        for (int i = 0; i < nodes->Count; i++) if (nodes[i] == currentNode) { curIndex = i; break; }
        if (curIndex == -1) return nullptr;

        for (int j = curIndex + 1; j < nodes->Count; j++) {
            if (!nodes[j]->isAlbumHeader) {
                setCurrentNode(nodes[j]);
                return nodes[j]->photoPath;
            }
        }
        return nullptr;
    }

    int PhotoList::shuffle()
    {
        // shuffle: choose a random photo index and set it current
        if (nodes->Count == 0) return -1;
        System::Random^ r = gcnew System::Random();
        int idx = r->Next(0, nodes->Count);
        int attempts = 0;
        while (nodes[idx]->isAlbumHeader && attempts < nodes->Count) { idx = r->Next(0, nodes->Count); attempts++; }
        if (nodes[idx]->isAlbumHeader) return -1;
        setCurrentNode(nodes[idx]);
        return idx;
    }

    bool PhotoList::isEmpty()
    {
        // isEmpty: returns true if no non-header photos exist
        for each (Node^ n in nodes) if (!n->isAlbumHeader) return false;
        return true;
    }

    void PhotoList::toggleAlbumExpansion(int displayIndex, ListBox^ lb)
    {
        // toggleAlbumExpansion: flip expansion flag for album header at
        // displayIndex and refresh the ListBox
        int idx = 0;
        for (int i = 0; i < nodes->Count; i++)
        {
            Node^ n = nodes[i];
            if (n->isAlbumHeader)
            {
                if (idx == displayIndex) { n->isExpanded = !n->isExpanded; if (lb != nullptr) populatePhotoList(lb); return; }
                idx++;
                if (n->isExpanded)
                {
                    int j = i + 1;
                    while (j < nodes->Count && !nodes[j]->isAlbumHeader && nodes[j]->albumName == n->albumName)
                    {
                        idx++; j++;
                    }
                }
            }
            else if (n->albumName == nullptr)
            {
                if (idx == displayIndex) return;
                idx++;
            }
        }
    }

    void PhotoList::setCurrentNode(Node^ node)
    {
        // setCurrentNode: set pointer if node exists in the collection
        if (node == nullptr) return;
        if (nodes->Contains(node))
        {
            currentNode = node;
        }
    }

    PhotoList::Node^ PhotoList::getNodeAtDisplayIndex(int displayIndex, ListBox^ lb)
    {
        // getNodeAtDisplayIndex: convert a UI display index into the
        // corresponding nodes[] entry (handles expanded albums)
        if (lb == nullptr) return nullptr;
        int idx = 0;
        for (int i = 0; i < nodes->Count; i++)
        {
            Node^ n = nodes[i];
            if (n->isAlbumHeader)
            {
                if (displayIndex == idx) return n;
                idx++;
                if (n->isExpanded)
                {
                    int j = i + 1;
                    while (j < nodes->Count && !nodes[j]->isAlbumHeader && nodes[j]->albumName == n->albumName)
                    {
                        if (displayIndex == idx) return nodes[j];
                        idx++; j++;
                    }
                }
            }
            else if (n->albumName == nullptr)
            {
                if (displayIndex == idx) return n;
                idx++;
            }
        }
        return nullptr;
    }

}
