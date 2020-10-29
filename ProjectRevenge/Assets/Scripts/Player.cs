using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public class Player : MonoBehaviour
{
    public GaugeBar HPBar;

    protected Animator mAnimator;
    protected Rigidbody2D mRigidbody2D;
    protected Vector2 moveDir;

    [SerializeField]
    protected float Speed; // 움직일 때의 속도
    [SerializeField]
    protected float JumpSpeed; // 점프 시 주어지는 힘

    [SerializeField]
    protected int MaxHP; // 최대 체력
    private int mCurrentHP; // 현재 체력

    [SerializeField]
    protected AttackArea attackArea;
    [SerializeField]
    protected int AttackPower;

    protected bool mbAttackState; // 공격 상태 여부
    public bool mbDead;
    private bool mbHit;
    public float MinX, MaxX;
    public float newMinX;

    public bool mbEnter;
    public bool mbRide;
    private bool mbInteractUp;
    private bool mbInteractDown;
    private int floor;
    public bool mbGround;

    // Start is called before the first frame update
    protected void Start()
    {
        mCurrentHP = MaxHP;
        mAnimator = GetComponent<Animator>();
        mRigidbody2D = GetComponent<Rigidbody2D>();

        HPBar.ShowGauge(mCurrentHP, MaxHP);

        mbDead = false;
        mbAttackState = false;
        mbHit = false;
        mbEnter = false;
        mbRide = false;
        mbInteractUp = false;
        mbInteractDown = false;
        mbGround = false;

        floor = 1;

        attackArea.SetDamage(AttackPower);
    }

    // Update is called once per frame
    protected void Update()
    {
        transform.position = new Vector3(Mathf.Clamp(transform.position.x, MinX, MaxX), transform.position.y, 0);
        UnityEngine.Debug.Log(mCurrentHP);
        UnityEngine.Debug.Log(floor);
            
        if (!mbDead)
        {
            float horizontal = Input.GetAxis("Horizontal");

            if (horizontal > 0)
            {
                transform.rotation = Quaternion.Euler(0, 0, 0);
                mAnimator.SetBool(AnimHash.Walk, true);
            }
            else if (horizontal < 0)
            {
                // 캐릭터 몸체 회전
                transform.rotation = Quaternion.Euler(0, 180, 0);
                mAnimator.SetBool(AnimHash.Walk, true);
            }
            else
            {
                mAnimator.SetBool(AnimHash.Walk, false);
            }

            mRigidbody2D.velocity = new Vector2(horizontal * Speed, mRigidbody2D.velocity.y); //이동시 힘을 주는 방향

            if (Input.GetButtonDown("Attack"))
            {
                mAnimator.SetBool(AnimHash.Attack, true);
                mbAttackState = true;
            }

            if (Input.GetButtonUp("Attack"))
            {
                mAnimator.SetBool(AnimHash.Attack, false);
            }

            if (Input.GetKeyDown(KeyCode.UpArrow))
            {
                mbInteractUp = true;
            }

            if(Input.GetKeyDown(KeyCode.DownArrow))
            {
                mbInteractDown = true;
            }
        }
    }

    public void Hit(int damage)
    {
        UnityEngine.Debug.Log("Hit " + damage);
        mCurrentHP -= damage;
        mbHit = true;
        HitMotion(mbHit);

        if (mCurrentHP <= 0)
        {
            mCurrentHP = 0;
            mAnimator.SetTrigger("Die");
            mRigidbody2D.velocity = Vector2.zero;
            mbDead = true;
        }

        HPBar.ShowGauge(mCurrentHP, MaxHP);
    }

    protected void AttackState()
    {
        mbAttackState = false;
    }

    public void HitMotion(bool isHit)
    {
        isHit = mbHit;

        if (isHit)
        {
            mAnimator.SetBool(AnimHash.HitBody, true);
            mbHit = false;
        }
    }

    public void EndHitMotion()
    {
        if (!mbHit)
        {
            mAnimator.SetBool(AnimHash.HitBody, false);
        }
    }

    protected void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("CameraTrigger"))
        {
            mbEnter = true;
            MinX = newMinX;
        }
    }

    protected void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Vehicle"))
        {
            if (mbInteractUp)
            {
                UnityEngine.Debug.Log("Ride");
                mbRide = true;
            }

            UnityEngine.Debug.Log("Vehicle");
        }

        if (collision.gameObject.CompareTag("Potal"))
        {
            if (mbInteractUp)
            {
                gameObject.transform.position = new Vector3(transform.position.x, transform.position.y + 2.02f, 0);
                UnityEngine.Debug.Log("up");
                mbInteractUp = false;
                floor++;
            }

            if (mbInteractDown)
            {
                if (floor > 1)
                {
                    gameObject.transform.position = new Vector3(transform.position.x, transform.position.y - 2.02f, 0);
                    UnityEngine.Debug.Log("down");
                    mbInteractDown = false;
                    floor--;
                }
            }

            UnityEngine.Debug.Log("Potal");
        }
    }

    protected void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.CompareTag("Ground"))
        {
            mbGround = true;
        }
    }

    protected void OnCollisionExit2D(Collision2D collision)
    {
        if(collision.gameObject.CompareTag("Ground"))
        {
            mbGround = false;
        }
    }
}
