//! @file VQS.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: VQS

#ifndef VQS_H
#define VQS_H

#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix44.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class VQS;

  template<typename Real>
  VQS<Real> Inverse(VQS<Real> const &);

  //! @ingroup Math_classes
  //!
  //! @class VQS
  //!
  //! @brief Vector, quaternion, scalar structure
  //!
  //! This is an alternate to matrices to represent
  //! transformations in 3D space. In this class we have a translation component
  //! (Vector), orientation (Quaternion) and scaling component (Scalar). The main
  //! advantage over a 4x4 matrix is using a quaternion for orientation, arguably
  //! a more complete and robust method for orientation representation. 
  //!
  //! Some disatvantages include: we are restricted to the types of transformations
  //! we can do. We cannot shear or reflect for example. Also We must use uniform
  //! scaling.
  //!
  //! @author Frank B. Hart
  //! @date 4/10/2015
  template<typename Real>
  class VQS
  {
  public:
    //! Default constructor set to identity
    VQS() : m_q(static_cast<Real>(1.0), 
                static_cast<Real>(0.0), 
                static_cast<Real>(0.0), 
                static_cast<Real>(0.0)), 
            m_s(static_cast<Real>(1.0)) {m_v.Zero();}
    VQS(Vector4<Real> const & a_v, Quaternion<Real> const & a_q, Real a_s) :
      m_v(a_v), m_q(a_q), m_s(a_s) {}
    ~VQS() {}

    //Copy operations
    VQS(VQS<Real> const & a_other) : m_v(a_other.m_v), m_q(a_other.m_q), m_s(a_other.m_s) {}
    VQS<Real>& operator=(VQS<Real> const &);

    //Comparison
    bool operator==(VQS<Real> const & a_other) const;
    bool operator!=(VQS<Real> const & a_other) const;

    //! Ensure a valid VQS.
    void MakeValid();

    //! Make identity.
    void Identity();

    //! Set VQS based on an affine matrix
    void Set(Matrix44<Real> const &);
    void Set(Vector4<Real> const &, Quaternion<Real> const &, Real);
    void SetV(Vector4<Real> const &);
    void SetQ(Quaternion<Real> const &);
    void SetS(Real);

    //! Translation update data
    void UpdateV(Vector4<Real> const &);

    //! Quaternion update data
    void UpdateQ(Quaternion<Real> const &);

    //! Scalar update data
    void UpdateS(Real);

    //! VQS structures concatenate left to right.
    VQS<Real> operator* (VQS<Real> const &) const;
    VQS<Real>& operator*= (VQS<Real> const &);

    //! Point transformations also apply translation.
    Vector4<Real> TransformPoint(Vector4<Real> const &);

    //! Vector transformations do not apply translation.
    Vector4<Real> TransformVector(Vector4<Real> const &);

    //! Point transformations also apply translation.
    Vector4<Real>& TransformPointSelf(Vector4<Real>&);

    //! Vector transformations do not apply translation.
    Vector4<Real>& TransformVectorSelf(Vector4<Real>&);

    //! Apply translation to Vector4.
    Vector4<Real> Translate(Vector4<Real> const &) const;

    //! Apply rotation to Vector4.
    Vector4<Real> Rotate(Vector4<Real> const &) const;

    //! Apply scale to Vector4.
    Vector4<Real> Scale(Vector4<Real> const &) const;

    //! Apply translation to Vector4.
    void TranslateSelf(Vector4<Real>&) const;

    //! Apply rotation to Vector4.
    void RotateSelf(Vector4<Real>&) const;

    //! Apply scale to Vector4.
    void ScaleSelf(Vector4<Real>&) const;

    //! Inverse.
    const VQS& Inverse();

    //! Inverse.
    template<typename T>
    friend VQS<T> Inverse(VQS<T> const &);

    //Returns
    void Get(Vector4<Real>& a_v, Quaternion<Real>& a_q, Real& a_s) const;

    //! Conversion to Matrix.
    void GetMatrix(Matrix44<Real>&) const;
    Vector4<Real> const & V()	  const	{ return m_v; }
    Quaternion<Real> const & Q() const	{ return m_q; }
    Real S()	                  const	{ return m_s; }

  private:
    //Data members
    Vector4<Real>		m_v;		//translation
    Quaternion<Real>	m_q;		//rotation
    Real		          m_s;		//scale
  };

  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Set(Matrix44<Real> const & a_m)
  {
    m_v.m_x = a_m[12];
    m_v.m_y = a_m[13];
    m_v.m_z = a_m[14];

    a_m.GetQuaternion(m_q);

    //Just use matrix x-scaling
    m_s = sqrt((a_m[0] * a_m[0]) + (a_m[4] * a_m[4]) + (a_m[8] * a_m[8]));

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetV(Vector4<Real> const & a_v)
  {
    m_v = a_v;
    m_v.m_V[3] = static_cast<Real>(0.0);

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::SetQ()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetQ(Quaternion<Real> const & a_q)
  {
    m_q = a_q;
    m_q.MakeValid();

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::SetS()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetS(Real a_s)
  {
    m_s = (a_s < Dg::EPSILON) ? static_cast<Real>(0.0) : a_s;

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateV()
  //--------------------------------------------------------------------------------
  //		Update translation vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateV(Vector4<Real> const & a_v)
  {
    m_v += a_v;
    m_v.m_w = static_cast<Real>(0.0);

  }	//End: VQS<Real>::UpdateV()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateQ()
  //--------------------------------------------------------------------------------
  //		Update quaternion
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateQ(Quaternion<Real> const & a_q)
  {
    m_q *= a_q;

  }	//End: VQS<Real>::UpdateQ()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateS()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateS(Real a_s)
  {
    m_s *= a_s;

  }	//End: VQS<Real>::UpdateS()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool VQS<Real>::operator==(VQS<Real> const & a_other) const
  {
    return !((a_other.m_v != m_v)
      || (a_other.m_q != m_q)
      || (a_other.m_s != m_s));

  }	//End: VQS<Real>::operator==();


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator!=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool VQS<Real>::operator!=(VQS<Real> const & a_other) const
  {
    return ((a_other.m_v != m_v)
      || (a_other.m_q != m_q)
      || (a_other.m_s != m_s));

  }	//End: VQS<Real>::operator!=();


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real>& VQS<Real>::operator=(VQS<Real> const & a_other)
  {
    m_v = a_other.m_v;
    m_q = a_other.m_q;
    m_s = a_other.m_s;

    return *this;

  }	//End: VQS<Real>::operator=();


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::MakeValid
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::MakeValid()
  {
    //Clean vector
    m_v.m_V[3] = static_cast<Real>(0.0);

    //Clean quaternion
    m_q.MakeValid();

    //Clean scale;
    if (m_s < Dg::EPSILON)
      m_s = static_cast<Real>(0.0);

  }	//End: VQS<Real>::MakeValid()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Identity()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Identity()
  {
    m_v.Zero();

    m_q.Identity();

    m_s = static_cast<Real>(1.0);

  }	//End: VQS<Real>::Identity()


  //--------------------------------------------------------------------------------
  //	@	TransformPoint()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> VQS<Real>::TransformPoint(Vector4<Real> const & a_v)
  {
    Vector4<Real> result(a_v);

    //Scale
    result.m_x *= m_s;
    result.m_y *= m_s;
    result.m_z *= m_s;

    //Rotate;
    m_q.RotateSelf(result);

    //Translate
    result += m_v;

    return result;

  }	//End: TransformPoint()


  //--------------------------------------------------------------------------------
  //	@	TransformPointSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& VQS<Real>::TransformPointSelf(Vector4<Real> & a_v)
  {
    //Scale
    a_v.m_x *= m_s;
    a_v.m_y *= m_s;
    a_v.m_z *= m_s;

    //Rotate;
    m_q.RotateSelf(a_v);

    //Translate
    a_v += m_v;

    return a_v;

  }	//End: TransformPointSelf()


  //--------------------------------------------------------------------------------
  //	@	TransformVector()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> VQS<Real>::TransformVector(Vector4<Real> const & a_v)
  {
    Vector4<Real> result(a_v);

    //Scale
    result.m_V[0] *= m_s;
    result.m_V[1] *= m_s;
    result.m_V[2] *= m_s;

    //Rotate;
    m_q.RotateSelf(result);

    return result;

  }	//End: TransformVector()


  //--------------------------------------------------------------------------------
  //	@	TransformVectorSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& VQS<Real>::TransformVectorSelf(Vector4<Real> & a_v)
  {
    //Scale
    a_v.m_x *= m_s;
    a_v.m_y *= m_s;
    a_v.m_z *= m_s;

    //Rotate;
    m_q.RotateSelf(a_v);

    return a_v;

  }	//End: TransformVectorSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Translate()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> VQS<Real>::Translate(Vector4<Real> const & a_v) const
  {
    Vector4<Real> result(a_v);

    result += m_v;

    return result;

  }	//End: VQS<Real>::Translate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Rotate()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> VQS<Real>::Rotate(Vector4<Real> const & a_v) const
  {
    Vector4<Real> result(a_v);

    m_q.RotateSelf(result);

    return result;

  }	//End: VQS<Real>::Rotate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Scale()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> VQS<Real>::Scale(Vector4<Real> const & a_v) const
  {
    Vector4<Real> result(a_v);

    result.m_x *= m_s;
    result.m_y *= m_s;
    result.m_z *= m_s;

    return result;

  }	//End: VQS<Real>::Scale()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::TranslateSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::TranslateSelf(Vector4<Real>& a_v) const
  {
    a_v.x += m_v.x;
    a_v.y += m_v.y;
    a_v.z += m_v.z;

  }	//End: VQS<Real>::TranslateSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::RotateSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::RotateSelf(Vector4<Real>& a_v) const
  {
    //Rotate;
    m_q.RotateSelf(a_v);

  }	//End: VQS<Real>::RotateSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::ScaleSelf()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::ScaleSelf(Vector4<Real>& a_v) const
  {
    //Scale
    a_v.x *= m_s;
    a_v.y *= m_s;
    pa_v.z *= m_s;

  }	//End: VQS<Real>::ScaleSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Set(Vector4<Real>    const & a_v, 
                      Quaternion<Real> const & a_q, 
                      Real                     a_s)
  {
    m_v = a_v;
    m_q = a_q;
    m_s = a_s;

    MakeValid();

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Get(Vector4<Real>&    a_v, 
                      Quaternion<Real>& a_q, 
                      Real&             a_s) const
  {
    a_v = m_v;
    a_q = m_q;
    a_s = m_s;

  }	//End: VQS<Real>::Get()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Get()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::GetMatrix(Matrix44<Real> & a_out) const
  {
    a_out.Rotation(m_q);

    a_out.m_V[0] *= m_s;
    a_out.m_V[1] *= m_s;
    a_out.m_V[2] *= m_s;
    a_out.m_V[4] *= m_s;
    a_out.m_V[5] *= m_s;
    a_out.m_V[6] *= m_s;
    a_out.m_V[8] *= m_s;
    a_out.m_V[9] *= m_s;
    a_out.m_V[10] *= m_s;

    a_out.m_V[12] = m_v.m_x *= m_s;
    a_out.m_V[13] = m_v.m_y *= m_s;
    a_out.m_V[14] = m_v.m_z *= m_s;

  }	//End: VQS<Real>::Get()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator*()
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real> VQS<Real>::operator*(VQS<Real> const & a_rhs) const
  {
    VQS<Real> result(*this);

    //Combine translation vectors
    result.m_v += (m_q.Rotate(a_rhs.m_v)*m_s);

    //Combine quaternions
    result.m_q *= a_rhs.m_q;

    //Combine scales
    result.m_s *= a_rhs.m_s;

    //Return result
    return result;

  }	//End: VQS<Real>::operator*()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Operator*=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real>& VQS<Real>::operator*=(VQS<Real> const & a_rhs)
  {
    //Combine translation vectors
    m_v += (m_q.Rotate(a_rhs.m_v)*m_s);

    //Combine quaternions
    m_q *= a_rhs.m_q;

    //Combine scales
    m_s *= a_rhs.m_s;

    return *this;

  }	//End: VQS<Real>::operator*=()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Inverse()
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real> const & VQS<Real>::Inverse()
  {
    //The method to find a VQS inverse
    //[1/m_s*(m_q-1*(-m_v)*m_q), m_q-1, 1/m_s]

    //Inverse scale
    m_s = static_cast<Real>(1.0) / m_s;

    //Inverse quaternion
    m_q.Inverse();

    //Inverse vector
    m_v = m_q.Rotate(-m_v) * m_s;

    return *this;

  }	//End: Inverse()


  //--------------------------------------------------------------------------------
  //	@	Inverse()
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real> Inverse(VQS<Real> const & a_other)
  {
    VQS<Real> temp;

    //Inverse scale
    temp.m_s = static_cast<Real>(1.0) / a_other.m_s;

    //Inverse quaternion
    temp.m_q = Inverse(a_other.m_q);

    //Inverse vector
    temp.m_v = temp.m_q.Rotate(-a_other.m_v) * temp.m_s;

    return temp;

  }	//End: Inverse()
}

#endif