using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HellBeast : Enemy
{
    public bool mbAttack;
    public FireBallPool fireBallPool;
    public Transform FireBallPos;
    private bool isHit;

    // Start is called before the first frame update
    new void Start()
    {
        base.Start();

        mbAttack = false;
        isHit = false;
    }

    private void Update()
    {
        HPBar.transform.position = HPBarPos.position;

        if (mbDead)
        {
            HPBar.gameObject.SetActive(false);
        }

        if (isHit)
        {
            transform.position = new Vector3(Random.Range(MinX, MaxX), transform.position.y, 0);
            mState = eEnemyState.Idle;
        }
    }

    public void ResetTarget()
    {
        mTarget = null;
    }

    public void LongDistanceAttack(bool isEnter)
    {
        if (isEnter && !isHit)
        {
            mState = eEnemyState.Attack;
            UnityEngine.Debug.Log("Attack");
        }
        else if (!isEnter)
        {
            mAnimator.SetBool(AnimHash.Attack, false);
            mState = eEnemyState.Idle;
        }
        else if (isHit && isEnter)
        {
            mAnimator.SetBool(AnimHash.Attack, false);
            mState = eEnemyState.Hit;
            isHit = false;
        }
    }

    private void FireStart()
    {
        FireBall fireBall = fireBallPool.GetFromPool();
        fireBall.transform.position = FireBallPos.position;
    }

    public new void Hit(int damage)
    {
        base.Hit(damage);

        isHit = true;
    }

    //맞는 애니메이션 실행
    public new void HitMotion(bool isHit)
    {
        base.HitMotion(isHit);
    }

    //맞는 애니메이션 끝내기
    public new void EndHitMotion()
    {
        base.EndHitMotion();
    }

    public new void DeadMotion(bool mbDead)
    {
        base.DeadMotion(mbDead);
    }

    //적이 죽으면 사라짐
    public new void AfterDead()
    {
        base.AfterDead();
    }
}
