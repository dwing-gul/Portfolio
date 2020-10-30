using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBall : MonoBehaviour
{
    private Player Target;
    private Rigidbody2D mRGBody;
    public float Speed;

    // Start is called before the first frame update
    void Awake()
    {
        Target = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        mRGBody = GetComponent<Rigidbody2D>();
    }

    private void Update()
    {
        Vector2 direction = Target.transform.position - transform.position;
        float diagonal = Mathf.Sqrt((direction.x * direction.x) + (direction.y * direction.y));
        float degree = Mathf.Abs(Mathf.Asin(direction.y / diagonal) * Mathf.Rad2Deg);
        if (direction.x < 0)
        {
            transform.rotation = Quaternion.Euler(0, 0, degree);
        }
        else
        {
            transform.rotation = Quaternion.Euler(0, 180, degree);
        }
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
