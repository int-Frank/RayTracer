#include "Camera.h"

void Camera::GenerateMatrix()
{
  vec4 trans;
  m_matrix.GetRow(3, trans);
  m_matrix.Rotation(m_roll, m_pitch, m_yaw, Dg::EulerOrder::YZX);
  m_matrix.SetRow(3, trans);
}


void Camera::SetScreen(float a_ar, float a_projDist)
{
  if (a_ar > 0.0f)
  {
    m_ar = a_ar;
  }

  if (a_projDist > 0.0f)
  {
    m_projDist = a_projDist;
  }
}


void Camera::SetYPR(float a_yaw,
                    float a_pitch,
                    float a_roll)
{
  m_yaw = a_yaw;
  m_pitch = a_pitch;
  m_roll = a_roll;

  Dg::WrapAngle(m_yaw);
  Dg::ClampNumber((-Dg::PI_f * 0.499f), (Dg::PI_f * 0.499f), m_pitch);
  Dg::WrapAngle(m_roll);

  GenerateMatrix();
}


void Camera::UpdateYPR(float a_yaw,
                       float a_pitch,
                       float a_roll)
{
  m_yaw += a_yaw;
  m_pitch += a_pitch;
  m_roll += a_roll;

  Dg::WrapAngle(m_yaw);
  Dg::ClampNumber((-Dg::PI_f * 0.499f), (Dg::PI_f * 0.499f), m_pitch);
  Dg::WrapAngle(m_roll);

  GenerateMatrix();
}


void Camera::MoveForward(float a_dx)
{
  vec4 trans, forward;
  m_matrix.GetRow(0, forward);
  m_matrix.GetRow(3, trans);
  m_matrix.SetRow(3, ((forward * a_dx) + trans));
  m_matrix.GetRow(3, trans);
}


void Camera::MoveLeft(float a_dx)
{
  vec4 trans, left;
  m_matrix.GetRow(1, left);
  m_matrix.GetRow(3, trans);
  m_matrix.SetRow(3, ((left * a_dx) + trans));
}


void Camera::MoveUp(float a_dx)
{
  vec4 trans, up;
  m_matrix.GetRow(2, up);
  m_matrix.GetRow(3, trans);
  m_matrix.SetRow(3, ((up * a_dx) + trans));
}


void Camera::MoveWorldUp(float a_dx)
{
  vec4 trans;
  vec4 worldUp(0.0f, 0.0f, 1.0f, 0.0f);
  m_matrix.GetRow(3, trans);
  m_matrix.SetRow(3, ((worldUp * a_dx) + trans));
}


void Camera::GetCornerRays(vec4 & a_ray00,
                           vec4 & a_ray01,
                           vec4 & a_ray10,
                           vec4 & a_ray11,
                           vec4 & a_origin)
{
  vec4 hLeft, hUp, forward;
  m_matrix.GetRow(2, hLeft);
  m_matrix.GetRow(1, hUp);
  m_matrix.GetRow(0, forward);
  hLeft *= (0.5f * m_ar);
  hUp *= 0.5f;
  forward *= m_projDist;

  a_ray00 = forward - hUp + hLeft;
  a_ray10 = forward - hUp - hLeft;
  a_ray01 = forward + hUp + hLeft;
  a_ray11 = forward + hUp - hLeft;
  m_matrix.GetRow(3, a_origin);
}