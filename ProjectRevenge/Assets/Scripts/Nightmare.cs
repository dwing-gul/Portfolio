using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Nightmare : MonoBehaviour
{
    private Rigidbody2D mRigidbody2D;
    private Animator mAnimator;
    private Player player;
    public bool Ride;
    public float MoveSpeed;
    public float MinX, MaxX;

    // Start is called before the first frame update
    void Start()
    {
        mRigidbody2D = GetComponent<Rigidbody2D>();
        mAnimator = GetComponent<Animator>();
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        Ride = false;
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = new Vector3(Mathf.Clamp(transform.position.x, MinX, MaxX), transform.position.y, 0);

        if(transform.position.x == MaxX)
        {
            Ride = false;
            player.mbRide = false;
        }

        if (player.mbRide)
        {
            player.transform.SetParent(transform);
            player.gameObject.SetActive(false);
            Ride = true;
        }
        else
        {
            player.transform.SetParent(null);
            player.gameObject.SetActive(true);
        }

        if (Ride)
        {
            mAnimator.SetBool(AnimHash.Walk, true);
            mRigidbody2D.velocity = transform.right * MoveSpeed;
        }
        else
        {
            mAnimator.SetBool(AnimHash.Walk, false);
            mRigidbody2D.velocity = Vector2.zero;
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("HitTrigger"))
        {
            collision.SendMessage("Hit", 100, SendMessageOptions.DontRequireReceiver);
        }
    }
}
