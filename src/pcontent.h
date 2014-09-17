// pcontent.h
// The content of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCONTENT_H
#define PCONTENT_H

#include <PFoundation/parray.h>

class PXmlElement;
class PBook;


struct PContentEntry
{
    friend class PContent;

    puint32  level;      // Start from 1, the first level.
    pchar   *text;
    puint32  pageNumber;

private:
    PContentEntry() {}
    ~PContentEntry() {}
};

class P_DLLEXPORT PContent
{
    friend class PBook;

public:
    PContent(PBook *book);
    ~PContent();

    P_INLINE puint32 numberOfEntries() { return m_entries.count(); }
    P_INLINE PContentEntry *entry(puint32 index) { return m_entries[index]; }

private: 
    pbool unpack(const PXmlElement *xmlElement);
    void clear();

private:
    PArray<PContentEntry *> m_entries;
    PBook *m_book;
};


#endif // !PCONTENT_H
