/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qrenderattachment_p.h"
#include "qrenderattachment.h"
#include "qtexture.h"
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QRenderAttachmentPrivate::QRenderAttachmentPrivate()
    : QNodePrivate()
    , m_texture(Q_NULLPTR)
    , m_attachmentPoint(QRenderAttachment::ColorAttachment0)
    , m_mipLevel(0)
    , m_layer(0)
    , m_face(QRenderAttachment::CubeMapNegativeX)
{
}

void QRenderAttachment::copy(const QNode *ref)
{
    QNode::copy(ref);
    const QRenderAttachment *rA = static_cast<const QRenderAttachment*>(ref);
    d_func()->m_attachmentPoint = rA->d_func()->m_attachmentPoint;
    setTexture(qobject_cast<QAbstractTextureProvider *>(QNode::clone(rA->d_func()->m_texture)));
}

QRenderAttachment::QRenderAttachment(QNode *parent)
    : QNode(*new QRenderAttachmentPrivate, parent)
{
}

QRenderAttachment::~QRenderAttachment()
{
    QNode::cleanup();
}

/*! \internal */
QRenderAttachment::QRenderAttachment(QRenderAttachmentPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

void QRenderAttachment::setAttachmentPoint(QRenderAttachment::AttachmentPoint attachmentPoint)
{
    Q_D(QRenderAttachment);
    if (attachmentPoint != d->m_attachmentPoint) {
        d->m_attachmentPoint = attachmentPoint;
        emit attachmentPointChanged(attachmentPoint);
    }
}

QRenderAttachment::AttachmentPoint QRenderAttachment::attachmentPoint() const
{
    Q_D(const QRenderAttachment);
    return d->m_attachmentPoint;
}

void QRenderAttachment::setTexture(QAbstractTextureProvider *texture)
{
    Q_D(QRenderAttachment);
    if (texture != d->m_texture) {
        d->m_texture = texture;

        // Handle inline declaration
        if (!texture->parent())
            texture->setParent(this);
        emit textureChanged(texture);
    }
}

QAbstractTextureProvider *QRenderAttachment::texture() const
{
    Q_D(const QRenderAttachment);
    return d->m_texture;
}

void QRenderAttachment::setMipLevel(int level)
{
    Q_D(QRenderAttachment);
    if (d->m_mipLevel != level) {
        d->m_mipLevel = level;
        emit mipLevelChanged(level);
    }
}

int QRenderAttachment::mipLevel() const
{
    Q_D(const QRenderAttachment);
    return d->m_mipLevel;
}

void QRenderAttachment::setLayer(int layer)
{
    Q_D(QRenderAttachment);
    if (d->m_layer != layer) {
        d->m_layer = layer;
        emit layerChanged(layer);
    }
}

int QRenderAttachment::layer() const
{
    Q_D(const QRenderAttachment);
    return d->m_layer;
}

void QRenderAttachment::setFace(QRenderAttachment::CubeMapFace face)
{
    Q_D(QRenderAttachment);
    if (d->m_face != face) {
        d->m_face = face;
        emit faceChanged(face);
    }
}

QRenderAttachment::CubeMapFace QRenderAttachment::face() const
{
    Q_D(const QRenderAttachment);
    return d->m_face;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
