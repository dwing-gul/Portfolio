using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HitTrigger : MonoBehaviour
{
    public bool bIsHit;

    private void Start()
    {
        bIsHit = false;
    }

    public void Hit(int count)
    { 
        Debug.Log(count);
        bIsHit = true;
        if(bIsHit == true)
        {
            gameObject.SetActive(false);
        }
    }
}
