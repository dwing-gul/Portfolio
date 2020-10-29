using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimHash : MonoBehaviour
{
    // 애니메이션을 의미하는 변수들. readonly는 읽기 전용이다
    public static readonly int Walk = Animator.StringToHash("IsWalk");
    public static readonly int Jump = Animator.StringToHash("Jump");
    public static readonly int Attack = Animator.StringToHash("IsAttack");
    public static readonly int JumpAttack = Animator.StringToHash("IsJumpAttack");
    public static readonly int Shield = Animator.StringToHash("IsShield");
    public static readonly int HitBody = Animator.StringToHash("IsHit");
}
