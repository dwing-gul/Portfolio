using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovePlatform : MonoBehaviour
{
    public Transform StartPos;
    public Transform EndPos;
    private Transform DesPos;
    public float Speed;
    private float StopWatch;
    public float WaitTime;

    // Start is called before the first frame update
    void Start()
    {
        transform.position = StartPos.position;
        DesPos = EndPos;
        StopWatch = WaitTime;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        transform.position = Vector2.MoveTowards(transform.position, DesPos.position, Time.deltaTime * Speed);

        if(Vector2.Distance(transform.position, DesPos.position) <= 0.05f)
        {
            if (DesPos == EndPos)
            {
                StopWatch -= Time.deltaTime;
                if(StopWatch <= 0)
                {
                    DesPos = StartPos;
                    StopWatch = WaitTime;
                }
            }
            else
            {
                StopWatch -= Time.deltaTime;
                if (StopWatch <= 0)
                {
                    DesPos = EndPos;
                    StopWatch = WaitTime;
                }
            }
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.transform.CompareTag("Player"))
        {
            collision.transform.SetParent(transform);
        }
    }

    private void OnCollisionExit2D(Collision2D collision)
    {
        if (collision.transform.CompareTag("Player"))
        {
            collision.transform.SetParent(null);
        }
    }
}
