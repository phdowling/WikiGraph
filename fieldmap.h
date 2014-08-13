#ifndef FIELDMAP_H
#define FIELDMAP_H

#include <QHash>
#include <QPoint>
#include <QPair>


template<typename OriginClass>
class FieldMap
{
public:
    FieldMap(int gridSizeExponent = 6) :
        _gridSizeExponent(gridSizeExponent)
    {
    }

    virtual ~FieldMap()
    {
    }

    void add(OriginClass *origin, QPointF pos, int fieldCellRadius = 2)
    {
        const int x = toGridCoordinate(pos.x());
        const int y = toGridCoordinate(pos.y());
        const int startRow = y - fieldCellRadius;
        const int endRow = y + fieldCellRadius;
        const int startCol = x - fieldCellRadius;
        const int endCol = x + fieldCellRadius;


        for(int row = startRow; row < endRow; ++row) {
            for(int col = startCol; col < endCol; ++col) {
                const unsigned int posHash = calcPosHash(col, row);
                _map[posHash].addField(origin, this);
            }
        }
        _map[calcPosHash(x, y)].addOrigin(origin);
    }

    QList< QPair<OriginClass *, OriginClass *> > getInfluencingElements() const
    {
        QList< QPair<OriginClass*, OriginClass*> > influencingElementsList;

        typedef typename QHash<OriginClass *, QList<OriginClass *> >::const_iterator InFieldListIterator;
        for(InFieldListIterator hashIt = _inFieldList.constBegin(); hashIt != _inFieldList.constEnd(); ++hashIt) {
            typedef typename QList<OriginClass *>::const_iterator OriginClassListIterator;
            for(OriginClassListIterator listIt = hashIt.value().constBegin(); listIt != hashIt.value().constEnd(); ++listIt) {
                influencingElementsList.append(QPair<OriginClass *, OriginClass *>(hashIt.key(), *listIt));
            }
        }
        return influencingElementsList;
    }

private:
    friend class FieldGridCell;
    class FieldGridCell
    {
    public:
        void addOrigin(OriginClass *origin)
        {
            _originList.append(origin);
        }

        void addField(OriginClass *fieldOrigin, FieldMap *parent)
        {
            _fieldList.append(fieldOrigin);
            foreach(OriginClass *origin, _originList) {
                parent->_inFieldList[fieldOrigin].append(origin);
            }
        }

    private:
        QList<OriginClass *> _originList;
        QList<OriginClass *> _fieldList;
    };

    FieldMap(const FieldMap &other) {}
    const FieldMap &operator=(const FieldMap &assigned) {}

    inline int toGridCoordinate(int pos)
    {
        return (pos >> _gridSizeExponent);
    }

    inline unsigned int calcPosHash(int x, int y)
    {
        // 1. shift 16 left to remove the most significant bits (this will not erase sign bit).
        // 2. cast to unsigned to make the sign bit shiftable
        // 3. shift 16 right to get bits in pos 15 to 0
        // 4. merge 16 bit coordinates to 32 bit hash. Shift x to the high word to not overwrite y.
        const unsigned int x16Bit = (((unsigned int)(x << 16)) >> 16);
        const unsigned int y16Bit = (((unsigned int)(y << 16)) >> 16);
        return ((x16Bit << 16) | y16Bit);
    }

    QHash<unsigned int, FieldGridCell> _map;
    QHash<OriginClass *, QList<OriginClass *> > _inFieldList;
    const int _gridSizeExponent;
};



#endif // FIELDMAP_H
