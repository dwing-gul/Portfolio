using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HitTriggerManager : MonoBehaviour
{
    public HitTrigger Trigger;
    public float MinX, MaxX;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if(Trigger.bIsHit)
        {
            transform.position = new Vector3(Random.Range(MinX, MaxX), transform.position.y, 0);
        }
    }
}
