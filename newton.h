#ifndef NEWTON_H
#define NEWTON_H

#include <QPoint>
#include <QVector2D>

namespace Newton
{

inline QVector2D calculateAcceleration(QVector2D force, double mass)
{
    return (force/mass);
}

inline QVector2D calculateRepulsionForce(double repulsion, QVector2D pos1, QVector2D pos2, double mass1, double mass2)
{
    double distance = (pos1 - pos2).length();
    return (repulsion * mass1 * mass2 * ((pos1 - pos2) / (distance * distance * distance)));
}

inline QVector2D calculateAttractionForce(double attraction, QVector2D pos1, QVector2D pos2)
{
    return (-attraction * (pos1 - pos2));
}



} // end namespace Newton


#endif // NEWTON_H
