using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HellHoundController : Enemy
{
    private Nightmare nightmare;

    // Start is called before the first frame update
    new void Start()
    {
        base.Start();
        nightmare = GameObject.FindGameObjectWithTag("Vehicle").GetComponent<Nightmare>();
    }

    // Update is called once per frame
    new void Update()
    {
        base.Update();

        if (nightmare.Ride)
        {
            mState = eEnemyState.Move;
        }
    }

    private void OnCollisionEnter2D(Collision2D collider)
    {
        if (collider.gameObject.CompareTag("Vehicle"))
        {
            Hit(MaxHP);
        }
    }

    public new void Hit(int damage)
    {
        base.Hit(damage);
    }

    public new void HitMotion(bool isHit)
    {
        base.HitMotion(isHit);
    }

    public new void EndHitMotion()
    {
        base.EndHitMotion();
    }

    public new void DeadMotion(bool mbDead)
    {
        base.DeadMotion(mbDead);
    }

    public new void AfterDead()
    {
        base.AfterDead();
    }

    public new void EnterAttackArea(bool isEnter)
    {
        base.EnterAttackArea(isEnter);
    }

    public new void NotEarlyAttack()
    {
        base.NotEarlyAttack();
    }

    public new void SetTarget(Transform target)
    {
        base.SetTarget(target);
    }

    public new void RemoveTarget()
    {
        base.RemoveTarget();
    }
}
