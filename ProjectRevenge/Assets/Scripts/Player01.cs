using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player01 : Player
{
    // 실드 사용 상태 여부를 묻는 변수
    private bool mbShieldState;
    [SerializeField]
    private Enemy boss;

    // Start is called before the first frame update
    new void Start()
    {
        base.Start();
        mbShieldState = false;
        boss = GameObject.FindGameObjectWithTag("Boss").GetComponent<Enemy>();
    }

    // Update is called once per frame
    new void Update() 
    {
        base.Update();

        if (mbShieldState || mbAttackState) // 공격이나 실드 상태일 때 이동 불가
        {
            mRigidbody2D.velocity = new Vector2(0, mRigidbody2D.velocity.y);
        }

        if (Input.GetButtonDown("ShieldBlock"))
        {
            mAnimator.SetBool(AnimHash.Shield, true);
            mbShieldState = true;
        }

        if (Input.GetButtonUp("ShieldBlock"))
        {
            mAnimator.SetBool(AnimHash.Shield, false);
            mbShieldState = false;
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("FireBall"))
        {
            if (!mbShieldState)
            {
                Hit(boss.HitDamage);
            }
        }
    }
}
