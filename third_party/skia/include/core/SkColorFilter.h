/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkColorFilter_DEFINED
#define SkColorFilter_DEFINED

#include "SkBlendMode.h"
#include "SkColor.h"
#include "SkFlattenable.h"
#include "SkRefCnt.h"

class GrContext;
class GrFragmentProcessor;
class SkArenaAlloc;
class SkBitmap;
class SkColorSpace;
class SkColorSpaceXformer;
class SkRasterPipeline;

/**
 *  ColorFilters are optional objects in the drawing pipeline. When present in
 *  a paint, they are called with the "src" colors, and return new colors, which
 *  are then passed onto the next stage (either ImageFilter or Xfermode).
 *
 *  All subclasses are required to be reentrant-safe : it must be legal to share
 *  the same instance between several threads.
 */
class SK_API SkColorFilter : public SkFlattenable {
public:
    /**
     *  If the filter can be represented by a source color plus Mode, this
     *  returns true, and sets (if not NULL) the color and mode appropriately.
     *  If not, this returns false and ignores the parameters.
     */
    virtual bool asColorMode(SkColor* color, SkBlendMode* bmode) const;

    /**
     *  If the filter can be represented by a 5x4 matrix, this
     *  returns true, and sets the matrix appropriately.
     *  If not, this returns false and ignores the parameter.
     */
    virtual bool asColorMatrix(SkScalar matrix[20]) const;

    /**
     *  If the filter can be represented by per-component table, return true,
     *  and if table is not null, copy the bitmap containing the table into it.
     *
     *  The table bitmap will be in SkBitmap::kA8_Config. Each row corresponding
     *  to each component in ARGB order. e.g. row[0] == alpha, row[1] == red,
     *  etc. To transform a color, you (logically) perform the following:
     *
     *      a' = *table.getAddr8(a, 0);
     *      r' = *table.getAddr8(r, 1);
     *      g' = *table.getAddr8(g, 2);
     *      b' = *table.getAddr8(b, 3);
     *
     *  The original component value is the horizontal index for a given row,
     *  and the stored value at that index is the new value for that component.
     */
    virtual bool asComponentTable(SkBitmap* table) const;

    void appendStages(SkRasterPipeline*, SkColorSpace*, SkArenaAlloc*, bool shaderIsOpaque) const;

    enum Flags {
        /** If set the filter methods will not change the alpha channel of the colors.
        */
        kAlphaUnchanged_Flag = 1 << 0,
    };

    /** Returns the flags for this filter. Override in subclasses to return custom flags.
    */
    virtual uint32_t getFlags() const { return 0; }

    /**
     *  If this subclass can optimally createa composition with the inner filter, return it as
     *  a new filter (which the caller must unref() when it is done). If no such optimization
     *  is known, return NULL.
     *
     *  e.g. result(color) == this_filter(inner(color))
     */
    virtual sk_sp<SkColorFilter> makeComposed(sk_sp<SkColorFilter>) const { return nullptr; }

    SkColor filterColor(SkColor) const;
    SkColor4f filterColor4f(const SkColor4f&) const;

    /** Create a colorfilter that uses the specified color and mode.
        If the Mode is DST, this function will return NULL (since that
        mode will have no effect on the result).
        @param c    The source color used with the specified mode
        @param mode The blend that is applied to each color in
                        the colorfilter's filterSpan[16,32] methods
        @return colorfilter object that applies the src color and mode,
                    or NULL if the mode will have no effect.
    */
    static sk_sp<SkColorFilter> MakeModeFilter(SkColor c, SkBlendMode mode);

    /** Construct a colorfilter whose effect is to first apply the inner filter and then apply
     *  the outer filter to the result of the inner's.
     *  The reference counts for outer and inner are incremented.
     *
     *  Due to internal limits, it is possible that this will return NULL, so the caller must
     *  always check.
     */
    static sk_sp<SkColorFilter> MakeComposeFilter(sk_sp<SkColorFilter> outer,
                                                  sk_sp<SkColorFilter> inner);

    /** Construct a color filter that transforms a color by a 4x5 matrix. The matrix is in row-
     *  major order and the translation column is specified in unnormalized, 0...255, space.
     */
    static sk_sp<SkColorFilter> MakeMatrixFilterRowMajor255(const SkScalar array[20]);

    /** Construct a colorfilter that applies the srgb gamma curve to the RGB channels */
    static sk_sp<SkColorFilter> MakeLinearToSRGBGamma();

    /** Construct a colorfilter that applies the inverse of the srgb gamma curve to the
     *  RGB channels
     */
    static sk_sp<SkColorFilter> MakeSRGBToLinearGamma();

#if SK_SUPPORT_GPU
    /**
     *  A subclass may implement this factory function to work with the GPU backend. It returns
     *  a GrFragmentProcessor that implemets the color filter in GPU shader code.
     *
     *  The fragment processor receives a premultiplied input color and produces a premultiplied
     *  output color.
     *
     *  A null return indicates that the color filter isn't implemented for the GPU backend.
     */
    virtual sk_sp<GrFragmentProcessor> asFragmentProcessor(GrContext*,
                                                           SkColorSpace* dstColorSpace) const;
#endif

    bool affectsTransparentBlack() const {
        return this->filterColor(0) != 0;
    }

    SK_TO_STRING_PUREVIRT()

    SK_DECLARE_FLATTENABLE_REGISTRAR_GROUP()
    SK_DEFINE_FLATTENABLE_TYPE(SkColorFilter)

protected:
    SkColorFilter() {}

    sk_sp<SkColorFilter> makeColorSpace(SkColorSpaceXformer* xformer) const {
        return this->onMakeColorSpace(xformer);
    }
    virtual sk_sp<SkColorFilter> onMakeColorSpace(SkColorSpaceXformer*) const {
        return sk_ref_sp(const_cast<SkColorFilter*>(this));
    }

private:
    /*
     *  Returns 1 if this is a single filter (not a composition of other filters), otherwise it
     *  reutrns the number of leaf-node filters in a composition. This should be the same value
     *  as the number of GrFragmentProcessors returned by asFragmentProcessors's array parameter.
     *
     *  e.g. compose(filter, compose(compose(filter, filter), filter)) --> 4
     */
    virtual int privateComposedFilterCount() const { return 1; }

    virtual void onAppendStages(SkRasterPipeline*, SkColorSpace*, SkArenaAlloc*,
                                bool shaderIsOpaque) const = 0;

    friend class SkColorSpaceXformer;
    friend class SkComposeColorFilter;

    typedef SkFlattenable INHERITED;
};

#endif
