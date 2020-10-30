using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    public GaugeBarPool HPBarPool;
    protected GaugeBar HPBar;
    public Transform HPBarPos;

    protected Animator mAnimator;
    protected Rigidbody2D mRigidbody2D;
    [SerializeField]
    protected float MoveSpeed;

    protected eEnemyState mState;
    protected Transform mTarget;

    protected Coroutine mStateShiftRoutine;
    protected bool mAttackFlag;

    public int MaxHP;
    protected int mCurrentHP;

    public bool mbDead;
    protected int ForNotEarlyAttack; // 공격 애니메이션 없이 데미지 입는 것 방지를 위한 변수
    protected bool mbHit;
    public int HitDamage;
    public float MinX, MaxX;

    public enum eEnemyState
    {
        Idle,
        Move,
        Hit,
        Attack
    }

    // Start is called before the first frame update
    protected void Start()
    {
        mRigidbody2D = GetComponent<Rigidbody2D>();
        mAnimator = GetComponent<Animator>();

        mbDead = false;
        mbHit = false;

        mCurrentHP = MaxHP;
        ForNotEarlyAttack = 1;

        HPBar = HPBarPool.GetFromPool();
        HPBar.ShowGauge(mCurrentHP, MaxHP);

        mState = eEnemyState.Idle;
        StartCoroutine(AI());
    }

    protected void Update()
    {
        transform.position = new Vector3(Mathf.Clamp(transform.position.x, MinX, MaxX), transform.position.y, 0);
        HPBar.transform.position = HPBarPos.position;

        if(mbDead)
        {
            HPBar.gameObject.SetActive(false);
        }
    }

    //플레이어에게 피격 당할 때 적 캐릭의 체력 감소 함수
    public void Hit(int damage)
    {
        mCurrentHP -= damage;
        mbHit = true;
        Debug.Log(damage);
        HitMotion(mbHit);

        if (mCurrentHP <= 0)
        {
            mbDead = true;
            DeadMotion(mbDead);
        }

        HPBar.ShowGauge(mCurrentHP, MaxHP);
    }

    //맞는 애니메이션 실행
    public void HitMotion(bool isHit)
    {
        isHit = mbHit;

        if (isHit)
        {
            mState = eEnemyState.Hit;
            mbHit = false;
        }
    }

    //맞는 애니메이션 끝내기
    public void EndHitMotion()
    {
        if (!mbHit)
        {
            mAnimator.SetBool(AnimHash.HitBody, false);
        }
    }

    public void DeadMotion(bool mbDead)
    {
        mAnimator.SetTrigger("DieEnemy");
        mRigidbody2D.velocity = Vector2.zero;
    }

    //적이 죽으면 사라짐
    public void AfterDead()
    {
        if(mbDead)
        {
            gameObject.SetActive(false);
        }
    }

    //공격범위에 플레이어가 들어왔을때
    public void EnterAttackArea(bool isEnter)
    {
        mAttackFlag = isEnter;
        if (mAttackFlag)
        {
            mState = eEnemyState.Attack;

            try
            {
                //플레이어의 Hit함수에 데미지 전달
                if (mAttackFlag && mTarget.CompareTag("Player") && (ForNotEarlyAttack == 0))
                {
                    mTarget.gameObject.SendMessage("Hit", HitDamage, SendMessageOptions.DontRequireReceiver);
                    UnityEngine.Debug.Log(HitDamage);
                }
                else if (mAttackFlag && mTarget.CompareTag("Shield"))
                {
                    mTarget.gameObject.SendMessage("Hit", 0, SendMessageOptions.DontRequireReceiver);
                }
            }
            catch(NullReferenceException ex)
            {
                
            }
        }

        if(!mAttackFlag)
        {
            mAnimator.SetBool(AnimHash.Attack, false);
            mState = eEnemyState.Idle;
            ForNotEarlyAttack = 1;
        }
    }

    public void NotEarlyAttack()
    {
        ForNotEarlyAttack = 0;
    }

    public void SetTarget(Transform target)
    {
        mTarget = target;
        mState = eEnemyState.Move;
    }

    public void RemoveTarget()
    {
        mTarget = null;
        mState = eEnemyState.Idle;
    }

    protected IEnumerator AI()
    {
        while(true)
        {
            switch(mState)
            {
                case eEnemyState.Idle :
                    if(mStateShiftRoutine == null && !mbDead)
                    {
                        mAnimator.SetBool(AnimHash.Walk, false);
                        mRigidbody2D.velocity = Vector2.zero;
                    }
                    break;
                case eEnemyState.Move :
                    if (!mbDead)
                    {
                        Vector2 direction = mTarget.position - transform.position;
                        if (direction.x < 0)
                        {
                            transform.rotation = Quaternion.Euler(0, 0, 0);
                        }
                        else
                        {
                            transform.rotation = Quaternion.Euler(0, 180, 0);
                        }
                        mAnimator.SetBool(AnimHash.Walk, true);
                        mRigidbody2D.velocity = -transform.right * MoveSpeed;
                    }
                    break;
                case eEnemyState.Attack :
                    mAnimator.SetBool(AnimHash.Attack, true);
                    break;
                case eEnemyState.Hit :
                    mAnimator.SetBool(AnimHash.HitBody, true);
                    break;
            }
            yield return new WaitForSeconds(0.2f);
        }
    }
}
