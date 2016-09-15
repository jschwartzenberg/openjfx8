/*
 * Copyright (c) 2011, 2015, Oracle and/or its affiliates. All rights reserved.
 */
#include "config.h"

#include "NotImplemented.h"

#include "Image.h"
#include "CachedImage.h"
#include "DragActions.h"
#include "DragClient.h"
#include <wtf/Vector.h>
#include <WTFString.h>

#include "JavaEnv.h"
#include "DragClientJava.h"

namespace WebCore {

// ---- DragImage.h ---- //
IntSize dragImageSize(DragImageRef pr)
{
    return pr ? roundedIntSize(pr->size()) : IntSize();
}

DragImageRef scaleDragImage(DragImageRef pr, FloatSize scale)
{
    //TODO: pass to java
    notImplemented();
    return pr;
}

DragImageRef dissolveDragImageToFraction(DragImageRef pr, float delta)
{
    //TODO: pass to java
    notImplemented();
    return pr;
}

DragImageRef createDragImageFromImage(Image* img, ImageOrientationDescription)
{
    if(img)
        img->ref();
    return img;
}

DragImageRef createDragImageIconForCachedImage(CachedImage *cimg)
{
    if (cimg->hasImage()) return nullptr;
    return createDragImageFromImage(cimg->image(), ImageOrientationDescription(RespectImageOrientation)); // todo tav valid orientation?
}

void deleteDragImage(DragImageRef pr)
{
    if(pr)
        pr->deref();
}

DragImageRef createDragImageIconForCachedImageFilename(const String&)
{
    return 0;
}


DragClientJava::DragClientJava(const JLObject &webPage)
    : m_webPage(webPage)
{
}

DragClientJava::~DragClientJava()
{
}

void DragClientJava::dragControllerDestroyed()
{
    delete this;
}

void DragClientJava::willPerformDragDestinationAction(
    DragDestinationAction action,
    DragData& data)
{
    notImplemented();
}

void DragClientJava::willPerformDragSourceAction(
    DragSourceAction,
    const IntPoint&,
    DataTransfer& DataTransfer)
{
    notImplemented();
}

DragDestinationAction DragClientJava::actionMaskForDrag(DragData& data)
{
    //TODO: check input element and produce correct respond
    notImplemented();
    return DragDestinationActionAny;
}

//We work in window rather than view coordinates here
DragSourceAction DragClientJava::dragSourceActionMaskForPoint(const IntPoint& windowPoint)
{
    //TODO: check input element and produce correct respond
    notImplemented();
    return DragSourceActionAny;
}

void DragClientJava::startDrag(
    DragImageRef dragImage,
    const IntPoint& dragImageOrigin,
    const IntPoint& eventPos,
    DataTransfer& DataTransfer,
    Frame& frame,
    bool linkDrag)
{
    JNIEnv* env = WebCore_GetJavaEnv();
    static jmethodID mid = env->GetMethodID(
        PG_GetWebPageClass(env),
        "fwkStartDrag", "("
        "Ljava/lang/Object;"
        "II"
        "II"
        "[Ljava/lang/String;"
        "[Ljava/lang/Object;"
        ")V");
    ASSERT(mid);

    static JGClass clsString(env->FindClass("java/lang/String"));
    static JGClass clsObject(env->FindClass("java/lang/Object"));

    Vector<String> mimeTypes(DataTransfer.typesPrivate());
    JLObjectArray jmimeTypes(env->NewObjectArray(mimeTypes.size(), clsString, NULL));
    JLObjectArray jvalues(env->NewObjectArray(mimeTypes.size(), clsObject, NULL));
    CheckAndClearException(env); // OOME

    {
        //we are temporary changing DataTransfer security context
        //for transfer-to-Java purposes.

        DataTransferAccessPolicy actualJSPolicy = DataTransfer.policy();
        DataTransfer.setAccessPolicy(DataTransferAccessPolicy::Readable); //XXX DataTransferReadable);

        int index = 0;
        Vector<String>::const_iterator end = mimeTypes.end();
        for(Vector<String>::const_iterator i = mimeTypes.begin();
            end!=i;
            ++i, ++index)
        {
            String value( DataTransfer.getData(*i) );

            env->SetObjectArrayElement(
                jmimeTypes,
                index,
                (jstring)i->toJavaString(env));

            env->SetObjectArrayElement(
                jvalues,
                index,
                (jstring)value.toJavaString(env));
        }

        DataTransfer.setAccessPolicy(actualJSPolicy);
    }

    // Attention! [jimage] can be the instance of WCImage or WCImageFrame class.
    // The nature of raster is too different to make a conversion inside the native code.
    jobject jimage =
        dragImage && dragImage->javaImage()
        ? jobject(*(dragImage->javaImage()))
        : 0;

    env->CallVoidMethod(m_webPage, mid, jimage,
        eventPos.x() - dragImageOrigin.x(),
        eventPos.y() - dragImageOrigin.y(),
        eventPos.x(),
        eventPos.y(),
        jobjectArray(jmimeTypes),
        jobjectArray(jvalues) );
    CheckAndClearException(env);
}

DragImageRef DragClientJava::createDragImageForLink(
    URL& url,
    const String& label,
    Frame* frame)
{
    notImplemented();
    return 0;
}

} // namespace WebCore
