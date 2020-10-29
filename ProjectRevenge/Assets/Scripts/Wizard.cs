using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Wizard : Enemy
{
    public Transform FireBallPos;
    public FireBallPool fireBallPool;
    private bool wizardHit;
    private Player target;

    // Start is called before the first frame update
    new void Start()
    {
        base.Start();
        wizardHit = false;
        target = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
    }

    private new void Update()
    {
        base.Update();
    }

    public void LongDistanceAttack(bool isEnter)
    {
        if(isEnter && !wizardHit)
        {
            mState = eEnemyState.Attack;
            UnityEngine.Debug.Log("Attack");
        }
        else if(!isEnter)
        {
            mAnimator.SetBool(AnimHash.Attack, false);
            mState = eEnemyState.Idle;
        }
        else if(wizardHit && isEnter)
        {
            mAnimator.SetBool(AnimHash.Attack, false);
            mState = eEnemyState.Hit;
            wizardHit = false;
        }
    }

    public new void Hit(int damage)
    {
        base.Hit(damage);

        wizardHit = true;
    }

    private void FireStart()
    {
        FireBall fireBall = fireBallPool.GetFromPool();
        fireBall.transform.position = FireBallPos.position;
        //fireBall.transform.LookAt(target.transform.position);
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
