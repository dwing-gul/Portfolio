using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpearTrap : MonoBehaviour
{
    private HitTrigger trigger;
    public Transform StartPos;
    public Transform EndPos;
    private Transform DesPos;
    private Transform CurrentPos;
    public float FireSpeed;
    public float RecoverySpeed;
    private float StopWatch;
    public float WaitTime;

    // Start is called before the first frame update
    void Start()
    {
        trigger = GameObject.FindGameObjectWithTag("HitTrigger").GetComponent<HitTrigger>();
        transform.position = StartPos.position;
        DesPos = EndPos;
        StopWatch = WaitTime;
        CurrentPos = gameObject.transform;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (trigger.bIsHit == true)
        {
            if (DesPos == EndPos)
            {
                transform.position = Vector2.MoveTowards(transform.position, DesPos.position, Time.deltaTime * FireSpeed);

                if (Vector2.Distance(transform.position, DesPos.position) <= 0.05f)
                {
                    StopWatch -= Time.deltaTime;
                    if (StopWatch <= 0)
                    {
                        DesPos = StartPos;
                        StopWatch = WaitTime;
                    }
                }
            }

            if (DesPos == StartPos)
            {
                transform.position = Vector2.MoveTowards(transform.position, DesPos.position, Time.deltaTime * RecoverySpeed);

                if (Vector2.Distance(transform.position, DesPos.position) <= 0.05f)
                {
                    StopWatch -= Time.deltaTime;
                    if (StopWatch <= 0)
                    {
                        DesPos = EndPos;
                        StopWatch = WaitTime;
                        if(CurrentPos.position.x == StartPos.position.x)
                        {
                            trigger.gameObject.SetActive(true);
                            trigger.bIsHit = false;
                        }
                    }
                }
            }
        }
    }
}
