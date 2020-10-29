using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBall : MonoBehaviour
{
    private Rigidbody2D mRGBody;
    public float Speed;

    // Start is called before the first frame update
    void Start()
    {
        mRGBody = GetComponent<Rigidbody2D>();
        mRGBody.velocity = -transform.right * Speed;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            gameObject.SetActive(false);
        }

        if(collision.gameObject.CompareTag("AttackArea") || collision.gameObject.CompareTag("Shield") || collision.gameObject.CompareTag("Wall") || collision.gameObject.CompareTag("Ground"))
        {
            gameObject.SetActive(false);
        }
    }
}
