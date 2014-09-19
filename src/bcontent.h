// bcontent.h
// The content of the book.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the GPL license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef BCONTENT_H
#define BCONTENT_H

#include <PFoundation/parray.h>

class PXmlElement;
class BBook;


struct BContentEntry
{
    friend class BContent;

    puint32  level;      // Start from 1, the first level.
    pchar   *text;
    puint32  pageNumber;

private:
    BContentEntry() {}
    ~BContentEntry() {}
};

class P_DLLEXPORT BContent
{
    friend class BBook;

public:
    BContent(BBook *book);
    ~BContent();

    P_INLINE puint32 numberOfEntries() { return m_entries.count(); }
    P_INLINE BContentEntry *entry(puint32 index) { return m_entries[index]; }

private: 
    pbool unpack(const PXmlElement *xmlElement);
    void clear();

private:
    PArray<BContentEntry *> m_entries;
    BBook *m_book;
};


#endif // !BCONTENT_H
